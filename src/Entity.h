#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "vector.h"
#include <SDL.h>
#include "SDL_image.h"

/** @brief	Defines an alias representing the entity. */  
typedef struct Entity_S
{
	int inuse;
	Vec2d pos;
	Vec2d vel;
	Sprite *sprite;
	int frame;
	int x,y;
	int health, maxHealth;
	int inventory[4];
	int speed;
	int state;
	int velocityX,velocityY;
	void (think*)(struct Entity_S *self);

}Entity;

Entity *Entity_New();
void Entity_Free(Entity *ent);
void Entity_InitSystem();
void Entity_Draw(Entity* ent);
void Entity_Update();

#endif