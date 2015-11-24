#include "gamestate.h"
#ifndef TEXTURE_H
#include "texture.h"
#endif
#ifndef GEOMETRY_H
#include "geometry.h"
#endif
#include "sprite.h"

#define STEP_SIZE 6
#define PROXIMITY 300
#define COLLISION 2

extern int globalTime;

struct Sprite *enemy;
int enemyLives = 3;

extern struct Vec2d enemyPos, enemyVel;
extern Sint16 enemyRadius;
extern int enemyChasing;
extern int WINDOW_W;
extern int WINDOW_H;

void enemyUpdate(void *enemyParam){
	/* Enemy update logic */
	struct Sprite *enemy = (struct Sprite*) enemyParam;
	/* Update enemy using KEYBOARD */
	if(enemy->pos.y < 0){
		enemy->vel.y = STEP_SIZE;
	}

	if(enemy->pos.y > WINDOW_H - enemy->h){
		enemy->vel.y = -STEP_SIZE;
	}
  
	enemy->pos = add(enemy->pos, enemy->vel);
	if(globalTime % 6 == 0){
		enemy->currentFrame++;
		enemy->currentFrame %= 2;
	}
}
