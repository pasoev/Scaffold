#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include "gamestate.h"
#ifndef TEXTURE_H
#include "texture.h"
#endif
#include "text.h"
#ifndef GEOMETRY_H
#include "geometry.h"
#endif
#include "sprite.h"
#include "player.h"

#define PROXIMITY 300
#define COLLISION 2

int globalTime = 0;

extern struct Sprite *player;

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

TTF_Font *themeFont = NULL;
SDL_Texture *backgroundTexture = (void*)NULL;

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

static void renderBackground(struct GameState *game){
  DrawImage(backgroundTexture, 0, 0, 1280, 800, 0, SDL_FLIP_NONE, game->renderer);
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
  globalTime++;
  struct GameStateMachine *fsm = (struct GameStateMachine*) fsm_param;
  if(player->vel.y){
    player->pos = add(player->pos, player->vel);
    if(player->pos.y < 200){
      player->pos.y = 300;
      player->vel.y = 0;
    }
  }

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
    if(playerLives > 0){
      playerLives--;
      /* throw the player away from the enemy */
      player->pos = add(player->pos,
			add(subtract(enemyPos, player->pos), (struct Vec2d){100, 0}));
    }else{
      toGameoverMode(fsm);
    }
  }
}

void playDraw(struct GameState *state) {
  SDL_SetRenderDrawColor(state->renderer, 230, 120, 20, 255);
  renderBackground(state);
  renderScore(state, playerScore);
  renderLives(state, playerLives);
  /* DrawImageFrame(player->texture, player->pos.x, player->pos.y, player->w,
     player->h, 1, player->currentFrame, 0, flip, state->renderer); */

  drawSprite(player, state->renderer);

  /* and the enemy */
  filledCircleColor(state->renderer, enemyPos.x, enemyPos.y, enemyRadius, 0xFFFF00FF);
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
  LoadImage("graphics/background.png", &backgroundTexture, game->renderer);
}

int playOnEnter(struct GameState *state) {
  /* */
  int success = -1;
  playerLives = 3;
  initFonts();
     
  /* enter the player */
  player = malloc(sizeof(struct Sprite));
  if (player != NULL){
    int imgW, imgH;
    makeSprite(player, 6, DEFAULT_HIT_POINTS, (struct Vec2d){ 600, 300 }, (struct Vec2d){ 0, 0 });

    player->update = playerUpdate;
    player->texture = (void *)NULL;
    LoadImage("graphics/sheet.png", (SDL_Texture **)&(player->texture), state->renderer);
    
    SDL_QueryTexture(player->texture, NULL, NULL, &imgW, &imgH);
    player->w = imgW / player->numFrames;
    player->h = imgH;

    /* And the enemy */
    enemyPos.x = 400;
    enemyPos.y = 600;
    enemyVel.x = 1;
    enemyVel.y = 1;
    enemyRadius = 30;
    enemyChasing = 0;

    success = 0;
  }
  initBackground(state);
  return success;
}

int playOnExit(void) {
  puts("Exited playstate");
  SDL_DestroyTexture(player->texture);
  SDL_DestroyTexture(backgroundTexture);
  /* TTF_CloseFont(themeFont); */
  return 1;
}
