#include "Vector.h"
#include "Entity.h"
#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Entity *entList;
int numEnts;
int entMax = 1000;
Entity *Entity_New()
{
	Entity* ent = (Entity *)malloc(sizeof(Entity));
	ent->inuse = 1;
	Vec2d pos = 0;
	Vec2d vel = 0;
	Sprite *sprite = NULL;
	ent->frame = NULL;
	ent->x = 0;
	ent->y = 0;
	ent->health = 0;
	ent->maxHealth = 0;
	ent->inventory[4] = 0;
	ent->speed = 0;
	ent->state = 0;
	ent->velocityX = 0;
	ent->velocityY = 0;

	return ent;
}
void Entity_Free(Entity *ent)
{
	ent->inuse = 0;

	sprite_Free(ent->sprite);

}
void Entity_InitSystem()
{
	int i;

	entList = (Entity *)malloc(sizeof(Entity)*entMax);
	memset(entList,0,sizeof(Sprite)*entMax);

	for(i = 0; i < entMax; i++)
	{
		Entity_Free(&entList[i]);
	}

	numEnts = 0;
}
void Entity_DrawAll(Entity* ent);
void Entity_UpdateAll();
