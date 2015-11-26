/* #include <SDL2/SDL2_gfxPrimitives.h> */
#include <stdio.h>
#include "gamestate.h"
#ifndef TEXTURE_H
#endif
#include "text.h"
#ifndef GEOMETRY_H
#include "geometry.h"
#endif
#include "sprite.h"
#include "ledge.h"
#include "player.h"
#include "enemy.h"

#define PROXIMITY 300
#define COLLISION 2

int globalTime = 0;

extern struct Sprite *player;
extern struct Sprite *enemy;
extern List *bullets;
extern List *ledges;
extern int playerScore;
extern int playerLives;

static struct Vec2d enemyPos, enemyVel;
static Sint16 enemyRadius;
static int enemyChasing;
extern int WINDOW_W;
extern int WINDOW_H;

#define CAPTION_OFFSET 20

int LIVES_LOC_X = CAPTION_OFFSET;
int LIVES_LOC_Y = CAPTION_OFFSET;

#define SCORE_LOC_X (WINDOW_W - CAPTION_OFFSET)
int SCORE_LOC_Y = CAPTION_OFFSET;

/* Camera */
int LEVEL_W = 1280 * 15;
int LEVEL_H = 800 * 2;
struct Camera camera = {{ 0, 0, 1280, 800}};

TTF_Font *themeFont = NULL;
SDL_Texture *backgroundTexture = (void*)NULL;
extern SDL_Texture *brickTexture;
extern SDL_Texture *bulletTexture;

int loadTextures(SDL_Renderer *renderer){
	LoadImage("graphics/sheet.png", (SDL_Texture**)&(player->texture), renderer);
	LoadImage("graphics/Biomechanical.png", (SDL_Texture**)&(enemy->texture), renderer);
	LoadImage("graphics/bullet.png", (SDL_Texture**)&bulletTexture, renderer);
	LoadImage("graphics/bricks.png", (SDL_Texture**)&brickTexture, renderer);
	LoadImage("graphics/landscape_3.png", (SDL_Texture**)&backgroundTexture, renderer);
}

static void renderScore(struct GameState *game, int score){
	char scoreTxt[10];
	sprintf(scoreTxt, "%d", score);
	DrawText(game->renderer, scoreTxt, SCORE_LOC_X - 20, SCORE_LOC_Y, themeFont, 0xFF, 0xFF, 0xFF);
}

static void renderLives(struct GameState *game, int lives){
	char livesTxt[2];
	sprintf(livesTxt, "%d\0", lives);
	DrawText(game->renderer, livesTxt, LIVES_LOC_X, LIVES_LOC_Y, themeFont, 0xFF, 0x0, 0x0);
}

static void renderBackground(struct GameState *game, struct Camera *camera){
	/* DrawImage(backgroundTexture, 0, 0, 1280, 800, 0, SDL_FLIP_NONE, game->renderer); */
	SDL_Rect body = {0, 0, 1280, 800};
	DrawImageOnCamera(backgroundTexture, body, 0, NULL, SDL_FLIP_NONE, game->renderer );
}

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

void toGameoverMode(struct GameStateMachine *fsm) {
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

void playerUpdateMouse(void *playerParam){
	struct Sprite *player = (struct Sprite*) playerParam;
	/* Update player using MOUSE 
	   struct Vec2d mousePos;
	   SDL_GetMouseState(&(mousePos.x), &(mousePos.y));
	   player->vel = subtract(mousePos, player->pos);
	   player->vel = divideByScalar(player->vel, 10);
	   player->pos = add(player->pos, player->vel);
	*/
}

void playUpdate(void *fsm_param) {
	struct GameStateMachine *fsm = (struct GameStateMachine*) fsm_param;
	if(player->vel.y){
		player->pos = add(player->pos, player->vel);
		if(player->pos.y < 100){
			if(globalTime % 6 == 0){
				player->pos.y = 300;
				player->vel.y = 0;
			}
		}
	}

	/* Look for Escape */
	if (isKeyDown(SDLK_ESCAPE)){
		toPauseMode(fsm);
	}

	/* Call update function for the player */
	player->update((void*)player);
	
	/* Center the camera over the dot */ // x = player->pos.x + 5;
	camera.rect.x = ( player->pos.x + player->w / 2 ) - WINDOW_W / 2;
	camera.rect.y = ( player->pos.y + player->h / 2 ) - WINDOW_H / 2;

	/* Keep the camera in bounds */
	if( camera.rect.x < 0 )
		{ 
			camera.rect.x = 0;
		}
	if( camera.rect.y < 0 )
		{
			camera.rect.y = 0;
		}
	if( camera.rect.x > LEVEL_W - camera.rect.w )
		{
			camera.rect.x = LEVEL_W - camera.rect.w;
		}
	if( camera.rect.y > LEVEL_H - camera.rect.h )
		{
			camera.rect.y = LEVEL_H - camera.rect.h;
		}
	
	updateBullets(bullets);

	enemy->update((void*)enemy);
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
		if(playerLives > 0){
			playerLives--;
			/* throw the player away from the enemy */
			player->pos = add(player->pos,
					  add(subtract(enemyPos, player->pos), (struct Vec2d){100, 0}));
		}else{
			toGameoverMode(fsm);
		}
	}
	globalTime++;
}

void playDraw(struct GameState *state) {
	SDL_SetRenderDrawColor(state->renderer, 230, 120, 20, 255);
	renderBackground(state, &camera);
	renderLedges(ledges, 10, state->renderer);
	renderScore(state, playerScore);
	renderLives(state, playerLives);
	drawSprite(player, &camera, state->renderer);
	drawSprite(enemy, &camera, state->renderer);
	drawBullets(bullets, state->renderer);
	/* and the enemy */
	/* filledCircleColor(state->renderer, enemyPos.x, enemyPos.y, enemyRadius, 0xFFFF00FF);*/
	SDL_SetRenderDrawColor(state->renderer, 230, 120, 20, 255);
}

int initFonts(void){
	if ( TTF_Init() < 0 ) {
		fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
		SDL_Quit();
		return(2);
	}
	themeFont = TTF_OpenFont("fonts/djserifi.ttf", 26);
}

int initBackground(struct GameState *game){
  
}

int playOnEnter(struct GameState *state) {
	/* */
	int success = 0;
	playerLives = 3;
	initFonts();
	/* enter the player */
	player = malloc(sizeof(struct Sprite));
	enemy = malloc(sizeof(struct Sprite));  
	if (player != NULL){
		int imgW, imgH;
		makeSprite(player, 6, DEFAULT_HIT_POINTS, (struct Vec2d){ 600, 300 }, (struct Vec2d){ 0, 0 });

		player->update = playerUpdate;
		makeSprite(enemy, 2, DEFAULT_HIT_POINTS, (struct Vec2d){ 750, 300 }, (struct Vec2d){ 0, 3 });

		enemy->update = enemyUpdate;
		player->texture = (void *)NULL;
		enemy->texture = (void*)NULL;
		loadTextures(state->renderer);
    
		SDL_QueryTexture(player->texture, NULL, NULL, &imgW, &imgH);
		player->w = imgW / player->numFrames;
		player->h = imgH;
		player->shooting = 0;

		SDL_QueryTexture(enemy->texture, NULL, NULL, &imgW, &imgH);
		enemy->w = imgW / enemy->numFrames;
		enemy->h = imgH;
	}else{
		success = -1;
	}

	bullets = malloc(sizeof *bullets);
	list_init(bullets, NULL);
	ledges = malloc(sizeof *ledges);
	list_init(ledges, destroyLedge);  
  
	initLedges(ledges, state->renderer);
  
	/* And the enemy */
	enemyPos.x = 400;
	enemyPos.y = 600;
	enemyVel.x = 1;
	enemyVel.y = 1;
	enemyRadius = 30;
	enemyChasing = 0;

	success = 0;
  
	initBackground(state);
	return success;
}

int playOnExit(void) {
	puts("Exited playstate");
	SDL_DestroyTexture(player->texture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(brickTexture);
	/* TTF_CloseFont(themeFont); */
	return 1;
}
