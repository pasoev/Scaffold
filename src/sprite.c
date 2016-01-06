#include "sprite.h"

/*
  The memory has to be allocated for the sprite before calling this
  function.
*/
int makeSprite(struct Sprite *sprite, int numFrames, int hitPoints,
	       struct Vec2d pos, struct Vec2d vel){
	((struct Sprite *)sprite)->pos = pos;
	((struct Sprite *)sprite)->vel = vel;
	((struct Sprite *)sprite)->accel = (struct Vec2d){0, 0};
	((struct Sprite *)sprite)->texture = (void *)NULL;
	((struct Sprite *)sprite)->numFrames = numFrames;
	((struct Sprite *)sprite)->currentFrame = 1;
	((struct Sprite *)sprite)->hitPoints = hitPoints;
	return 0;
}

void drawSprite(struct Sprite *sprite, struct Camera *camera,
		SDL_Renderer *renderer){
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int x, y;

	if (sprite->facing == LEFT){
		flip = SDL_FLIP_HORIZONTAL;
	}else{
		flip = SDL_FLIP_NONE;
	}

	x = sprite->pos.x - camera->rect.x;
	y = sprite->pos.y - camera->rect.y;

	SDL_Rect body = {x, y, sprite->w, sprite->h};
	/* DrawImageOnCamera(sprite->texture, body, NULL, 0, NULL, flip, renderer); */
	SDL_Rect camBody = {camera->rect.x, camera->rect.y, camera->rect.w,
			    camera->rect.h};
	/* DrawImageFrameOnCamera(sprite->texture, body, &camBody, 1,
	   sprite->currentFrame, 0, NULL, flip, renderer); */
	DrawImageFrame(sprite->texture, sprite->pos.x, sprite->pos.y, sprite->w,
		       sprite->h, 1, sprite->currentFrame, 0, flip, renderer);
}

public void update(float deltaTime) {
        stateTime += deltaTime;
        if (body == null) {
		updateMotionX(deltaTime);
		updateMotionY(deltaTime);

		// Move to new position
		position.x += velocity.x * deltaTime;
		position.y += velocity.y * deltaTime;
        } else {
		position.set(body.getPosition());
		rotation = body.getAngle() * MathUtils.radiansToDegrees;
        }
}

void updateMotionX(float deltaTime) {
        if (velocity.x != 0) {
		// Apply friction
		if (velocity.x > 0) {
			velocity.x = Math.max(velocity.x - friction.x * deltaTime, 0);
		} else {
			velocity.x = Math.min(velocity.x + friction.x * deltaTime, 0);
		}
        }
        // Apply acceleration
        velocity.x += acceleration.x * deltaTime;
        // Make sure the object's velocity does not exceed the
        // positive or negative terminal velocity
        velocity.x = MathUtils.clamp(velocity.x, -terminalVelocity.x, terminalVelocity.x);
}

void updateMotionY(float deltaTime) {
        if (velocity.y != 0) {
		// Apply friction
		if (velocity.y > 0) {
			velocity.y = Math.max(velocity.y - friction.y * deltaTime, 0);
		} else {
			velocity.y = Math.min(velocity.y + friction.y * deltaTime, 0);
		}
        }
        // Apply acceleration
        velocity.y += acceleration.y * deltaTime;
        // Make sure the object's velocity does not exceed the
        // positive or negative terminal velocity
        velocity.y = MathUtils.clamp(velocity.y, -terminalVelocity.y, terminalVelocity.y);
}
