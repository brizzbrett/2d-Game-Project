#include "Entity.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "simple_logger.h"

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
	for(i = 0; i < entMax; i++)
	{
		if(entList[i].inuse)
		{
			continue;
		}
		memset(&entList[i],0,sizeof(Entity));
		entList[i].inuse = 1;
		numEnt++;
		if(numEnt > entMax)
		{
			fprintf(stderr, "Maximum Sprites Reached.\n");
			exit(1);
		}

		entList[i].draw = &sprite_Draw;
		entList[i].think = NULL;
		entList[i].update = NULL;
		entList[i].touch = &Entity_IntersectAll;

		entList[i].inuse = 1;
		entList[i].type = OTHER;
		entList[i].pos = p;
		entList[i].sprite = sprite_Load(file,fw,fh);
		entList[i].frame = 0;
		entList[i].health = 0;
		entList[i].maxHealth = 0;
		entList[i].nextThink = 0;
		entList[i].thinkRate = 0;

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

	if(!ent)return;
	if(!*ent)return;

	//self = *ent;
	(*ent)->inuse = 0;

	sprite_Free(&(*ent)->sprite);
	(*ent)->sprite = NULL;
	*ent = NULL;

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

void Entity_IntersectAll(Entity *a)
{
	int i;
	if(!a)return;

	for(i=0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(a == &entList[i])
		{
			continue;
		}
		if(Entity_Intersect(a, &entList[i]))
		{
			slog("Hitting...");
			entList[i].touch(&entList[i]);
		}
	}
	return;
}
int Entity_Intersect(Entity *a, Entity *b)
{
	SDL_Rect aB, bB;
	if ((!a) || (!b))
	{
		return 0;
	}
	aB = rect(a->pos.x,a->pos.y,a->bounds.w, a->bounds.h);
	bB = rect(b->pos.x,b->pos.y,b->bounds.w, b->bounds.h);
	if(rect_intersect(aB, bB))
		return 1;
	return 0;
}
Entity *Entity_GetByID(int id)
{
	Uint32 i;
	for(i=0; i < numEnt; i++)
	{
		if(entList[i].id == id)
		{
			return &entList[i];
		}
	}
	return NULL;
}
Entity *Entity_GetByType(EntityType type)
{
	Uint32 i;
	for(i=0; i < numEnt; i++)
	{
		if(entList[i].type == type)
		{
			return &entList[i];
		}
	}
	return NULL;
}
