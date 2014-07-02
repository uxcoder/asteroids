#ifndef __ASTEROIDS_H
#define __ASTEROIDS_H

#define WIDTH			800
#define HEIGHT			500





typedef struct gameobj{
	int id;
	float x, y;
	float vx, vy;
	float angle;
	float size;
	double lifetime;
	void (*draw)(struct gameobj *);
	void (*tick)(struct gameobj *);
} gameobj;








/*
 * Protorypes
 *
 */
void init(void);
void draw_ship(gameobj *);
void draw_asteroid(gameobj *);
void draw_bullet(gameobj *o);

void tick_ship(gameobj *o);
void tick_asteroid(gameobj *o);
void tick_bullet(gameobj *o);

void render(void); 
void speckey_down(int key, int x, int y);
void speckey_up(int key, int x, int y);
void key_down(unsigned char key, int x, int y);
void key_up(unsigned char key, int x, int y);
void onidle(void);

#endif
