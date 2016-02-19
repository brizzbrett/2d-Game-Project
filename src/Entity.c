#include "Entity.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"

static Entity *entList; /**<static global Entity List*/
static Uint32 entMax = 100; /**<static unsigned 32-bit integer of maximum entities*/
Uint32 numEnt = 0; /**<unsigned 32-bit integer numEnt*/

/**
 * @brief	Creates a new reference to an Entity.
 * @return	a new Entity.
 */
Entity *Entity_New()
{
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < numEnt; i++)
	{
		if(entList[i].inuse)
		{
			continue;
		}
		memset(&entList[i],0,sizeof(Entity));
		entList[i].inuse = 1;
		return &entList[i];
	}
	return NULL;
}

/**
 * @brief	Frees up the memory allocated to the Entity that is input
 * @param	**ent	If not null, a pointer to the entity pointer.
 */
void Entity_Free(Entity **ent)
{
	Entity *self; /**<alias for *ent*/

	if(!ent)return;
	if(!*ent)return;

	self = *ent;
	self->inuse = 0;

	sprite_Free(&self->sprite);

}

/**   
 * @brief	Closes the Entity System by freeing up memory allocated to it. 
 */
void Entity_CloseSystem()
{
	Entity *ent; /**<alias for *ent*/
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < entMax ;i++)
	{
		ent= &entList[i];
        Entity_Free(&ent);
	}
	free(entList);
}

/**
 * @brief	Initialises the Entity System.
 * @param	ent_Max	The maximum number of Entities.
 */
void Entity_InitSystem(Uint32 ent_Max)
{
	int i;

	if(entMax == 0)
	{
		printf("Max Entity == 0\n");
		return;
	}
	entList = (Entity *)malloc(sizeof(Entity)*ent_Max);
	memset(entList,0,sizeof(Entity)*ent_Max);
	entMax = ent_Max;
	atexit(Entity_CloseSystem);
}

/**
 * @brief	Entity draw.
 * @param	*ent				If not null, the entity.
 * @param	frame				The frame in the spritesheet thats being drawn.
 * @param	*renderer			The renderer being drawn to.
 * @param	drawPos				The position on the screen the entity is being drawn on.
 */
void Entity_DrawAll(Entity* ent, int frame, SDL_Renderer *renderer, Vec2d drawPos)
{
	SDL_Rect src; /**<source rectangle*/
	SDL_Rect dest;/**<destination rectangle*/

	src.x = frame%ent->sprite->fpl * ent->sprite->frameSize.x;
	src.y = frame/ent->sprite->fpl * ent->sprite->frameSize.y;
	src.w = ent->sprite->frameSize.x;
	src.h = ent->sprite->frameSize.y;

	dest.x = drawPos.x;
	dest.y = drawPos.y;
	dest.w = ent->sprite->frameSize.x;
	dest.h = ent->sprite->frameSize.y;

	SDL_RenderCopy(renderer,ent->sprite->image, &src, &dest);
}

/**   
 * @brief	The Think function for all entities. 
 */
void Entity_ThinkAll()
{
	Uint32 i;
	for(i = 0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(!entList[i].think)
		{
			continue;
		}
		entList[i].think(&entList[i]);
	}
}

/**   
 * @brief	The Update function that updates all entities every certain amount of time.  
 */
void Entity_UpdateAll()
{
	Uint32 i;
	for(i= 0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(!entList[i].update)
		{
			continue;
		}
		entList[i].update(&entList[i]);
	}
}
