#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include "gamestate.h"
#include "texture.h"
#ifndef GEOMETRY_H
#include "geometry.h"
#endif
#include "sprite.h"

#define PROXIMITY 300
#define COLLISION 2

struct Sprite *player;

static struct Vec2d enemyPos, enemyVel;
static Sint16 enemyRadius;
static int enemyChasing;
extern int WINDOW_W;
extern int WINDOW_H;

extern int isKeyDown(SDL_Scancode);

static struct GameState pauseState;
static struct GameState gameoverState;

void toPauseMode(struct GameStateMachine *fsm) {
	struct GameState *thisState = (struct GameState*) list_tail(fsm->gameStates)->data;
	pauseState = (struct GameState){
		&pauseUpdate,
			&pauseDraw,
			&pauseOnEnter,
			&pauseOnExit,
			GAME_STATE_PAUSE,
			thisState->renderer
	};
	fsm->pushState(fsm, &pauseState);
}

void togameoverMode(struct GameStateMachine *fsm) {
	struct GameState *thisState = (struct GameState*) list_tail(fsm->gameStates)->data;
	gameoverState = (struct GameState){
		&gameoverUpdate,
			&gameoverDraw,
			&gameoverOnEnter,
			&gameoverOnExit,
			GAME_STATE_GAMEOVER,
			thisState->renderer
	};
	fsm->changeState(fsm, &gameoverState);
}

void playerUpdate(void *playerParam){
	/* Player update logic */
	struct Vec2d mousePos;
	struct Sprite *player = (struct Sprite*) playerParam;
	SDL_GetMouseState(&(mousePos.x), &(mousePos.y));

	/* Update player */
	player->vel = subtract(mousePos, player->pos);
	player->vel = divideByScalar(player->vel, 10);
	player->pos = add(player->pos, player->vel);
}

void playUpdate(void *fsm_param) {
	struct GameStateMachine *fsm = (struct GameStateMachine*) fsm_param;

	/* Look for Escape */
	if (isKeyDown(SDLK_ESCAPE)){
		toPauseMode(fsm);
	}

	/* Call update function for the player */
	player->update((void*)player);

	/* Update enemy */

	/* wrop around horizontally */
	if (enemyPos.x - enemyRadius < 0 || enemyPos.x + enemyRadius > WINDOW_W){
		enemyVel.x = -enemyVel.x;
	}

	/* wrop around vertically */
	if (enemyPos.y - enemyRadius < 0 || enemyPos.y + enemyRadius > WINDOW_H){
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

	/* Check collision */
	if (distance <= COLLISION + player->w / 2 + enemyRadius){
		/* enemyPos = (struct Vec2d){WINDOW_W / 2, WINDOW_H / 2}; */
		togameoverMode(fsm);
	}
}

void playDraw(struct GameState *state) {
	SDL_SetRenderDrawColor(state->renderer, 230, 120, 20, 255);
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	/* render player */
	if (player->vel.x > 0){
		flip = SDL_FLIP_HORIZONTAL;
	}
	else{
		flip = SDL_FLIP_NONE;
	}
	DrawImage(player->texture, player->pos.x, player->pos.y, player->w,
		player->h, 0, flip, state->renderer);

	/* and the enemy */
	filledCircleColor(state->renderer, enemyPos.x, enemyPos.y, enemyRadius, 0xFFFF00FF);
	SDL_SetRenderDrawColor(state->renderer, 230, 120, 20, 255);
}

int playOnEnter(struct GameState *state) {
	/* enter the player */
	player = malloc(sizeof(struct Sprite));
	if (player != NULL){
		makeSprite(player, (struct Vec2d){ 600, 400 }, (struct Vec2d){ 20, 20 });

		player->update = playerUpdate;
		player->texture = (void *)NULL;
		LoadImage("graphics/goo.png", (SDL_Texture **)&(player->texture), state->renderer);
		SDL_QueryTexture(player->texture, NULL, NULL, &(player->w), &(player->h));

		/* And the enemy */
		enemyPos.x = 400;
		enemyPos.y = 600;
		enemyVel.x = 1;
		enemyVel.y = 1;
		enemyRadius = 30;
		enemyChasing = 0;
		return 0;
	}
	else{
		return -1;
	}
}

int playOnExit(void) {
	puts("Exited playstate");
	SDL_DestroyTexture(player->texture);
	return 1;
}
