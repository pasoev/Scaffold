/* 
 * File:   asteroids.c
 * Author: sergi
 *
 * Created on December 25, 2014, 3:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "asteroids.h"

#define PROGRAM_NAME "Asteroids"
/* Default resolution. */
int WINDOW_W = 800, WINDOW_H = 600;

int running = 1;
const Uint8 *keystates;

void processEvents(void) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = 0;
      break;
    }
    keystates = SDL_GetKeyboardState(0);
  }
}

int isKeyDown(SDL_Scancode key) {
  if (keystates != 0) {
    return keystates[key];
  }
  return 0;
}

void destroy(void* param) {
  printf("That's how you kill it");
}

void queryResolution(int *x, int *y) {
  SDL_DisplayMode current;
  int i = 0;
  for (i = 0; i < SDL_GetNumVideoDisplays(); ++i) {

    int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

    if (should_be_zero != 0) {
      /* In case of error... */
      printf("Could not get display mode for video display #%d: %s",
	     i, SDL_GetError());
    } else {
      /* On success, assign globals and print the current display mode. */
      *x = current.w;
      *y = current.h;
      printf("Display #%d: current display mode is %dx%dpx @ %dhz. \n",
	     i, current.w, current.h, current.refresh_rate);
    }
  }
}

void quit(SDL_Window *window, SDL_Renderer *renderer);

int main(int argc, char** argv) {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0) {
	  /* Dynamically set the windos size to full screen*/
    queryResolution(&WINDOW_W, &WINDOW_H);
    SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H,
				SDL_WINDOW_SHOWN,
				&window, &renderer);
    SDL_SetWindowTitle(window, PROGRAM_NAME);
  }

  if (window != 0) {
    running = 1;
  } else {
    puts(SDL_GetError());
  }

  /* Manage states*/
  List states;
  list_init(&states, &destroy);
  struct GameStateMachine *fsm = &(struct GameStateMachine){
    &states, &pushState, &changeState, &popState, &popAllStates
  };

  struct GameState menuState = {
    &menuUpdate, &menuDraw, &menuOnEnter, &menuOnExit,
    GAME_STATE_MENU, renderer
  };
  
  fsm->changeState(fsm, &menuState);
  
  while (running) {
    if(list_size(fsm->gameStates) > 0){
      struct GameState *currentState = (struct GameState*) (list_tail(fsm->gameStates)->data);
      processEvents();
      currentState->update(fsm);
      SDL_RenderClear(renderer);
      currentState->render(currentState);
      SDL_RenderPresent(renderer);
    }else{
      /* All states have been popped off. Quit */
      running = 0;
      quit(window, renderer);
    }
  }
  
  return (EXIT_SUCCESS);
}

void quit(SDL_Window *window, SDL_Renderer *renderer){
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}
