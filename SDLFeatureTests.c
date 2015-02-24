// SDLFeatureTests.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <stdio.h>
#include <SDL.h>


int bakmain(int argc, char* argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		window = SDL_CreateWindow("Tests", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	}
	else{
		perror(SDL_GetError());
		return -1;
	}

	if (window != NULL){
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}

	if (renderer == NULL){
		perror("Couldn't initialize the renderer.\n");
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_Delay(5000);
	return 0;
}

