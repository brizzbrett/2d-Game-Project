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
Entity *Entity_New(char file[], int fw, int fh, Vec2d p)
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

	numEnt++;
	if(numEnt + 1 >= entMax)
	{
		fprintf(stderr, "Maximum Sprites Reached.\n");
		exit(1);
	}

	entList[i].inuse = 1;
	entList[i].pos = p;
	entList[i].sprite = sprite_Load(file,fw,fh);
	entList[i].frame = 0;
	entList[i].health = 0;
	entList[i].maxHealth = 0;
	entList[i].nextThink = 0;
	entList[i].thinkRate = 0;

	return &entList[i];
}

/**
 * @brief	Frees up the memory allocated to the Entity that is input
 * @param	**ent	If not null, a pointer to the entity pointer.
 */
void Entity_Free(Entity **ent)
{
	//Entity *self; /**<alias for *ent*/

	if(!ent)return;
	if(!*ent)return;

	//self = *ent;
	(*ent)->inuse = 0;

	sprite_Free(&(*ent)->sprite);
	(*ent)->sprite = NULL;

}

/**   
 * @brief	Closes the Entity System by freeing up memory allocated to it. 
 */
void Entity_CloseSystem()
{
	Entity *ent; /**<alias for *ent*/
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < entMax; i++)
	{
		ent= &entList[i];
        Entity_Free(&ent);
	}
	free(entList);
	entList = NULL;	
	numEnt = 0;
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
void Entity_DrawAll()
{
	Uint32 i;
	for(i = 0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(!entList[i].draw)
		{
			continue;
		}
		entList[i].draw(entList[i].sprite, entList[i].frame, Graphics_GetActiveRenderer(), entList[i].pos);
	}
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
		vec2d_Add(entList[i].pos, entList[i].vel, entList[i].pos);
		if(!entList[i].update)
		{
			continue;
		}
		entList[i].update(&entList[i]);
	}
}
