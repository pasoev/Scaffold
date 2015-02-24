#ifndef _TEXT_H

#include "texture.h"


#include <SDL_ttf.h>

void DrawText(SDL_Renderer *renderer, char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
#endif
