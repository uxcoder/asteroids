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


struct gameobj {
	int id;
	float x;
	float y;
	float angle;
	float size;
	void (*draw)(float, float);
};

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutCreateWindow("Asteroids");
	glutDisplayFunc(render);
	// glutSpecialFunc(onkeypressed);
	// glutIdleFunc(onidle);
	init();
	glutMainLoop();
	return EXIT_SUCCESS;
	return 0;
}


void init(void)
{
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, WIDTH, HEIGHT, 0);
	glEnable(GL_MULTISAMPLE);
	glPointSize(1.0);



	LLIST *list = create_list();

	struct gameobj ship;
	struct gameobj asteroid;
	struct gameobj bullet;

	ship.id = 100;
	asteroid.id = 200;
	bullet.id = 300;

	append_tolist(list, &ship);
	append_tolist(list, &bullet);
	append_tolist(list, &asteroid);

	delete_list(list);

	// o.draw = &draw_bullet;

}


void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_ship(WIDTH/2, HEIGHT/2, 0);
	draw_asteroid(300, 200, 0, 40);
	draw_bullet(600, 300);
	// (*o.draw)(600, 100);
	glutSwapBuffers();
}


void draw_ship(float x, float y, float angle)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(angle, 0, 0, 1);
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(0, -15);
	glVertex2f(8, 15);
	glVertex2f(-8, 15);
	glEnd();
	glPopMatrix();
}


void draw_asteroid(float x, float y, float angle, float size)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(angle, 0, 0, 1);
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++)
		glVertex2f(size * cos(2 * i * M_PI/5),  size *sin(2 * i * M_PI/5));
	glEnd();
	glPopMatrix();
}


void draw_bullet(float x, float y)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
}





































