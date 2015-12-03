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

struct Sprite *player;
int playerScore = 0;
int playerLives = 3;

extern struct Vec2d enemyPos, enemyVel;
extern Sint16 enemyRadius;
extern int enemyChasing;

extern int WINDOW_W; /* only for bullets */
extern int LEVEL_W;
extern int WINDOW_H;

extern int isKeyDown(SDL_Scancode);

/* Bullets */
struct Bullet{
  struct Vec2d pos;
  struct Vec2d vel;
};

List *bullets;
SDL_Texture *bulletTexture = (void *)NULL;

struct Vec2d gravity = {0, -1};

void initPlayer(void){}

void makeBullet(int x, int y, int dx){
  struct Bullet *bullet = malloc(sizeof(struct Bullet));
  bullet->pos.x = x;
  bullet->pos.y = y;
  bullet->vel.x = dx;
  bullet->vel.y = 0;

  list_ins_next(bullets, list_tail(bullets), bullet);
}

void shoot(struct Sprite* sprite){
  player->state = SHOOTING;
  if(player->currentFrame == 4){
    player->currentFrame = 5;
  }else{
    player->currentFrame = 4;
  }
  int x = 0, y = 0, dx = 0;
  if(player->vel.x < 0){
    x = player->pos.x + 5;
    y = player->pos.y + 20;
  }else{
    x = player->pos.x + 35;
    y = player->pos.y + 20;
  }
  dx = player->vel.x * 1.5;
  makeBullet(x, y, dx);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shooting.\n");
}

void updateBullets(List *bullets){
  if(list_size(bullets) > 0){
    ListElmt *elmt = list_head(bullets);
    struct Bullet *firstBullet = list_data(elmt);
    firstBullet->pos = add(firstBullet->pos, firstBullet->vel);

    if(firstBullet->pos.x > WINDOW_W || firstBullet->pos.x < 0){
      list_rem_next(bullets, NULL, (void **)&firstBullet);
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		  "Bullet disappeared. %d remaining.\n", list_size(bullets));
      if(firstBullet != NULL){
	free(firstBullet);
      }
    }

    for(elmt = list_head(bullets); elmt != NULL &&
	  !list_istail(elmt); elmt = list_next(elmt)){
      struct Bullet *nextBullet = list_data(list_next(elmt));
      nextBullet->pos = add(nextBullet->pos, nextBullet->vel);

      if(nextBullet->pos.x > WINDOW_W || nextBullet->pos.x < 0){
	list_rem_next(bullets, elmt, (void **)&nextBullet);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Bullet disappeared. %d remaining.\n", list_size(bullets));
	if(nextBullet != NULL){
	  free(nextBullet);
	}
      }
    }
  }
}

void playerUpdate(void *playerParam){
  /* Player update logic */
  struct Sprite *player = (struct Sprite*) playerParam;
  /* Update player using KEYBOARD */
  if(player->state != SHOOTING && player->state != JUMPING &&
     player->state != FALLING){
    if(isKeyDown(SDL_SCANCODE_LEFT)){
      player->vel = (struct Vec2d){-STEP_SIZE, 0};
      if(player->pos.x > 0){
	player->pos = add(player->pos, player->vel);
	player->state = WALKING;

	if(globalTime % 6 == 0){
	  player->currentFrame++;
	  player->currentFrame %= 4;
	}
      }
    }else if(isKeyDown(SDL_SCANCODE_RIGHT)){
      player->vel = (struct Vec2d){STEP_SIZE, 0};
      player->state = WALKING;
      if(player->pos.x < LEVEL_W - STEP_SIZE - player->w){
	player->pos = add(player->pos, player->vel);

	if(globalTime % 6 == 0){
	  player->currentFrame++;
	  player->currentFrame %= 4;
	}
      }
    }else{
      player->state = IDLE;
      player->currentFrame = 4;
    }
  }
  if(isKeyDown(SDL_SCANCODE_UP) && player->state != JUMPING &&
     player->state != FALLING){
    player->state = JUMPING;
    player->vel.y = -10;
  }

  if(isKeyDown(SDL_SCANCODE_SPACE) && player->state != WALKING){
    if(globalTime % 6 == 0){
      shoot(player);
    }else{
      player->state = IDLE;
      player->currentFrame = 4;
    }
  }

  if(player->state == JUMPING){
    struct Vec2d diff = subtract(player->vel, gravity);
    player->vel = subtract(player->vel, gravity);
    player->pos = add(player->pos, diff);

    if(player->vel.y >= 0){
      player->state = FALLING;
      player->vel.y = 10;
    }
  }

  if(player->state == FALLING){
    struct Vec2d diff = subtract(player->vel, multByScalar(gravity, -1));
    player->vel = subtract(player->vel, multByScalar(gravity, -1));
    player->pos = add(player->pos, diff);

    if(player->vel.y <= 0){
      player->state = IDLE;
      player->vel.y = 0;
    }
  }
}
void drawBullets(List *bullets, SDL_Renderer *renderer){
  ListElmt *elmt;
  for(elmt = list_head(bullets); elmt != NULL; elmt = list_next(elmt)){
    struct Bullet *bullet = (struct Bullet *)list_data(elmt);
    DrawImage(bulletTexture, bullet->pos.x, bullet->pos.y, 8, 8, 0,
	      SDL_FLIP_NONE, renderer );
  }
}
