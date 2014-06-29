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



void init(void)
{
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glEnable(GL_MULTISAMPLE);
    glPointSize(1.0);



    list = create_list();

    gameobj *ship = calloc(1, sizeof(gameobj));
    ship->id = 0;
    ship->x = WIDTH/2;
    ship->y = HEIGHT/2;
    ship->angle = 90;
    ship->draw = &draw_ship;
    ship->tick = &tick_ship;

    // gameobj *asteroid = calloc(1, sizeof(gameobj));
    // asteroid->id = 200;
    // asteroid->x = 600;
    // asteroid->y = 400;
    // asteroid->vx = -1;
    // asteroid->vy = -0.3;
    // asteroid->angle = 40;
    // asteroid->size = 50;
    // asteroid->draw = &draw_asteroid;
    // asteroid->tick = &tick_asteroid;

    append_tolist(list, ship);

    for (int i = 1; i <= 3; i++) {
        gameobj *a = calloc(1, sizeof(gameobj));
        float angle = random()%360/M_PI;
        a->id = i;
        a->x = random() % WIDTH;
        a->y = random() % HEIGHT;
        a->vx = cos(angle);
        a->vy = sin(angle);
        a->angle = angle*M_PI;
        a->size = random()%30 + 15;
        a->draw = &draw_asteroid;
        a->tick = &tick_asteroid;
        append_tolist(list, a);

    }


    gameobj *b = calloc(1, sizeof(gameobj));
    b->id = 100;
    b->x = -20 * cos(ship->angle) + ship->x;
    b->y = -20 * sin(ship->angle) + ship->y;
    b->vx = -cos(ship->angle);
    b->vy = -sin(ship->angle);
    // a->angle = angle*M_PI;
    // a->size = random()%30 + 15;
    b->draw = &draw_bullet;
    b->tick = &tick_bullet;
    append_tolist(list, b);

    // remove_item(list, 1);

    game.running = true;
    game.last_update = glutGet(GLUT_ELAPSED_TIME);

}


void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (NODE *node = list->head; node != NULL; node = node->next) {
        gameobj *o = (gameobj *)(node->data);
        o->tick(o);
        o->draw(o);
    }
    glutSwapBuffers();
}


void draw_ship(gameobj *o)
{
    glPushMatrix();
    glTranslatef(o->x, o->y, 0);
    glRotatef(o->angle, 0, 0, 1);
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
        glVertex2f(o->size * cos(2 * i * M_PI/5),  o->size *sin(2 * i * M_PI/5));
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
        o->x -= WIDTH-o->size;
    if (o->y > HEIGHT+o->size)
        o->y -= HEIGHT-o->size;
    if (o->x < -o->size)
        o->x += WIDTH+o->size;
    if (o->y < -o->size)
        o->y += HEIGHT+o->size;

}


void tick_bullet(gameobj *o)
{
    o->x += o->vx * DELTA / 100.0f, WIDTH;
    o->y += o->vy * DELTA / 100.0f, HEIGHT;
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
    if (key == SPACEBAR)
        keystate[FIRE] = 1;
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

























