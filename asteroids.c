/*
 * An asteroids clone
 * Copyright (C) 2014 Alexander Ariskin
 * Copyright (C) 2014 Igor Archipov
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "asteroids.h"
#include "llist.h"


#define NKEYS 		5
#define DELTA 		10

#define SPACEBAR 	32
#define ESC 		27


static struct {
	bool running;
	int last_update;
} game;


LLIST *list;


enum direction {LEFT, RIGHT, UP, DOWN, FIRE};
char keystate[NKEYS];


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutCreateWindow("Asteroids");
	glutDisplayFunc(render);
	glutSpecialFunc(speckey_down);
	glutSpecialUpFunc(speckey_up);
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);

	glutIdleFunc(onidle);
	init();
	glutMainLoop();

	delete_list(list);
	return EXIT_SUCCESS;

}


void remove_item(LLIST *list, int id)
{
	NODE *node;
	for (node = list->head; node != NULL; node = node->next) {
		if (((gameobj *)(node->data))->id == id) {
			if (node == list->tail) {
				node->prev->next = NULL;

			} else if (node == list->head) {
				node->next->prev = NULL;

			} else {
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}
			free(node);
			list->count--;
		}
	}
}


void remove_node(LLIST *list, NODE *node)
{
	if (node == list->tail && node == list->head) {
		list->head = NULL;
		list->tail = NULL;
	}  else if (node == list->head) {
		list->head = node->next;
		list->head->prev = NULL;
	} else if (node == list->tail) {
		list->tail = node->prev;
		list->tail->next = NULL;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	free(node);
	list->count--;
}


void init(void)
{
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, WIDTH, HEIGHT, 0);
	glEnable(GL_MULTISAMPLE);
	glPointSize(1.0);

	srandomdev();

	list = create_list();

	gameobj *ship = calloc(1, sizeof(gameobj));
	ship->id = 0;
	ship->x = WIDTH/2;
	ship->y = HEIGHT/2;
	ship->angle = 30;
	ship->draw = &draw_ship;
	ship->tick = &tick_ship;

	append_tolist(list, ship);
	int max_asteroids = 1;
	for (int i = 0; i < max_asteroids; i++) {
		gameobj *a = calloc(1, sizeof(gameobj));
		float angle = random()%360/M_PI;
		a->id = 1;
		a->x = random() % WIDTH;
		a->y = random() % HEIGHT;
		a->vx = cos(angle);
		a->vy = sin(angle);
		a->angle = angle * M_PI;
		a->size = 64;//random()%30 + 15;
		a->draw = &draw_asteroid;
		a->tick = &tick_asteroid;
		append_tolist(list, a);

	}


	// remove_item(list, 1);

	game.running = true;
	game.last_update = glutGet(GLUT_ELAPSED_TIME);

}


void spawn_bullet(int id)
{
	NODE *node = list->head;
	gameobj *ship = (gameobj *)(node->data);

	gameobj *b = calloc(1, sizeof(gameobj));
	b->id = -1;
	b->x = 20 * cos(ship->angle * M_PI/180.0) + ship->x;
	b->y = 20 * sin(ship->angle * M_PI/180.0) + ship->y;
	b->vx = cos(ship->angle * M_PI/180.0);
	b->vy = sin(ship->angle * M_PI/180.0);
	b->lifetime = 100;
	b->draw = &draw_bullet;
	b->tick = &tick_bullet;
	append_tolist(list, b);
}


void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (NODE *node = list->head; node != NULL; node = node->next) {
		gameobj *o = (gameobj *)(node->data);
		if (o->lifetime < 0 && o->id == -1)
			remove_node(list, node);
		else {
			o->tick(o);
			o->draw(o);
		}

	}
	glutSwapBuffers();
}


void draw_ship(gameobj *o)
{
	glPushMatrix();
	glTranslatef(o->x, o->y, 0);
	glRotatef(o->angle + 90, 0, 0, 1);
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(0, -15);
	glVertex2f(10, 15);
	glVertex2f(0, 10);
	glVertex2f(-10, 15);
	glEnd();
	glPopMatrix();
}


void draw_asteroid(gameobj *o)
{
	glPushMatrix();
	glTranslatef(o->x, o->y, 0);
	glRotatef(o->angle, 0, 0, 1);
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++)
		glVertex2f(o->size * cos(2 * i * M_PI/5),  o->size * sin(2 * i * M_PI/5));
	glEnd();
	glPopMatrix();
}


void draw_bullet(gameobj *o)
{
	glPushMatrix();
	glTranslatef(o->x, o->y, 0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
}


void tick_ship(gameobj *o)
{
	if (keystate[LEFT])
		o->angle += 0.5 * DELTA;
	if (keystate[RIGHT])
		o->angle -= 0.5 * DELTA;
	if (keystate[FIRE])
		spawn_bullet(-1);
}


void tick_asteroid(gameobj *o)
{
	if (o->angle < 360.0)
		o->angle += DELTA/100.0;
	else
		o->angle = 0.0;

	o->x += o->vx * DELTA / 10.0f, WIDTH;
	o->y += o->vy * DELTA / 10.0f, HEIGHT;

	if (o->x > WIDTH+o->size)
		o->x = -o->size;
	if (o->y > HEIGHT+o->size)
		o->y = -o->size;
	if (o->x < -o->size)
		o->x = WIDTH+o->size;
	if (o->y < -o->size)
		o->y = HEIGHT+o->size;

}


float dist(gameobj *a, gameobj *b)
{
	return (a->x - b->x) * (a->x - b->x) - (a->y - b->y) * (a->y - b->y);
}


void tick_bullet(gameobj *o)
{
	o->x += o->vx * DELTA / 3.0f, WIDTH;
	o->y += o->vy * DELTA / 3.0f, HEIGHT;

		
	o->lifetime--;

	
//	for (NODE *node = list->head; node != NULL; node = node->next) {
//		gameobj *a = (gameobj *)(node->data);
//		if (a->id == 1 && dist(a, o) < sqrt(a->size)) {
//			remove_node(list, node);	
//		}
//	}
}



void speckey_down(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_RIGHT:
			keystate[RIGHT] = 1;
			break;
		case GLUT_KEY_LEFT:
			keystate[LEFT] = 1;
			break;
		case GLUT_KEY_UP:
			//
			break;
		case GLUT_KEY_DOWN:
			//
			break;
	}
}


void speckey_up(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_RIGHT:
			keystate[RIGHT] = 0;
			break;
		case GLUT_KEY_LEFT:
			keystate[LEFT] = 0;
			break;
		case GLUT_KEY_UP:
			//
			break;
		case GLUT_KEY_DOWN:
			//
			break;
	}
}


void key_down(unsigned char key, int x, int y)
{
	if (key == SPACEBAR) {
		keystate[FIRE] = 1;
	}
	else if (key == ESC)
		exit(0);
}


void key_up(unsigned char key, int x, int y)
{
	if (key == SPACEBAR)
		keystate[FIRE] = 0;
}


void onidle(void)
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = now - game.last_update;

	if (elapsed > 1000/60.0 && game.running) {


		glutPostRedisplay();
		game.last_update = now;

	}
}

























