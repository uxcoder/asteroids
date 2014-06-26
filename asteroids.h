#ifndef __ASTEROIDS_H
#define __ASTEROIDS_H

#define WIDTH			800
#define HEIGHT			500


/*
 * Protorypes
 *
 */
void init(void);
void draw_ship(float x, float y, float angle);
void draw_asteroid(float x, float y, float angle, float size);
void draw_bullet(float x, float y);
void render(void); 


#endif