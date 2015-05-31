#include <SDL2/SDL.h>
#include "gamestate.h"
#ifndef TEXTURE_H
#include "texture.h"
#endif
#include "text.h"
#ifndef GEOMETRY_H
#include "geometry.h"
#endif
#include "sprite.h"

#define PROXIMITY 300
#define COLLISION 2

extern int globalTime;

struct Sprite *player;
int playerScore = 0;
int playerLives = 3;

extern struct Vec2d enemyPos, enemyVel;
extern Sint16 enemyRadius;
extern int enemyChasing;
extern int WINDOW_W;
extern int WINDOW_H;

extern int isKeyDown(SDL_Scancode);

void playerUpdate(void *playerParam){
  /* Player update logic */
  struct Sprite *player = (struct Sprite*) playerParam;

  /* Update player using KEYBOARD */
  if(!player->shooting && !player->vel.y){
    if(isKeyDown(SDL_SCANCODE_LEFT)){
      player->vel = (struct Vec2d){-3, 0};
      player->pos = add(player->pos, player->vel);
      player->walking = 1;

      if(globalTime % 6 == 0){
	player->currentFrame++;
	player->currentFrame %= 4;
      }
    }else if(isKeyDown(SDL_SCANCODE_RIGHT)){
      player->vel = (struct Vec2d){3, 0};
      player->walking = 1;
      player->pos = add(player->pos, player->vel);

      if(globalTime % 6 == 0){
	player->currentFrame++;
	player->currentFrame %= 4;
      }
    }else{
      player->walking = 0;
      player->currentFrame = 4;
    }
  }
  if(isKeyDown(SDL_SCANCODE_UP) && !(player->vel).y)
    {
      player->vel.y =-16;
    }
}
