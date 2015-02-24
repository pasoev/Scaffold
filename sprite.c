#include "sprite.h"

/* 
The memory has to be allocated for the sprite before calling this
function.
*/
int makeSprite(struct Sprite *sprite, struct Vec2d pos, struct Vec2d vel){
  ((struct Sprite *)sprite)->pos = pos;
  ((struct Sprite *)sprite)->vel = vel;
  ((struct Sprite *)sprite)->texture = (void *)NULL;

  return 0;
}
