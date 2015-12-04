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
extern int LEVEL_W;
extern int LEVEL_H;

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

  /* wrop around horizontally */
  if (enemyPos.x - enemyRadius < 0 || enemyPos.x + enemyRadius > LEVEL_W){
    enemyVel.x = -enemyVel.x;
  }

  /* wrop around vertically */
  if (enemyPos.y - enemyRadius < 0 || enemyPos.y + enemyRadius > LEVEL_H){
    enemyVel.y = -enemyVel.y;
  }

  /* The enemy needs to check if you're near */
  double distance = Vec2dLen(subtract(player->pos, enemyPos));
  if (distance <= PROXIMITY + player->w / 2 + enemyRadius){
    if (!enemyChasing){
      enemyChasing = 1;
      if (player->pos.x > enemyPos.x){
	enemyVel.x = -enemyVel.x;
      }
      if (player->pos.y > enemyPos.y){
	enemyVel.y = -enemyVel.y;
      }
    }
  }
  else{
    enemyChasing = 0;
  }
  enemyPos = add(enemyPos, enemyVel);

}
