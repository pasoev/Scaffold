#include "sprite.h"

/* 
   The memory has to be allocated for the sprite before calling this
   function.
*/
int makeSprite(struct Sprite *sprite, int numFrames, int hitPoints, struct Vec2d pos, struct Vec2d vel){
	((struct Sprite *)sprite)->pos = pos;
	((struct Sprite *)sprite)->vel = vel;
	((struct Sprite *)sprite)->texture = (void *)NULL;
	((struct Sprite *)sprite)->numFrames = numFrames;
	((struct Sprite *)sprite)->currentFrame = 1;
	((struct Sprite *)sprite)->hitPoints = hitPoints;
	return 0;
}

void drawSprite(struct Sprite *sprite, SDL_Renderer *renderer){
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (sprite->vel.x < 0){
		flip = SDL_FLIP_HORIZONTAL;
	}else{
		flip = SDL_FLIP_NONE;
	}

	DrawImageFrame(sprite->texture, sprite->pos.x, sprite->pos.y, sprite->w,
		       sprite->h, 1, sprite->currentFrame, 0, flip, renderer);
}
