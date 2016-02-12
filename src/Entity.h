#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "vector.h"


/** @brief	Defines an alias representing the entity. */  
typedef struct Entity_S
{
	int inuse;
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

#endif