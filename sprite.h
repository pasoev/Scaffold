/* 
 * File:   player.h
 * Author: sergi
 *
 * Created on December 31, 2014, 7:34 PM
 */

#ifndef SPRITE_H
#define	SPRITE_H

#ifndef GEOMETRY_H
#include "geometry.h"
#endif

#include <stdlib.h>

struct Sprite{
  int w, h;
  struct Vec2d pos, vel;
  void * texture;
  void (*update)(void *world);
};

int makeSprite(struct Sprite *sprite, struct Vec2d pos, struct Vec2d vel);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* PLAYER_H */

