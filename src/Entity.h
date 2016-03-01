#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Vector.h"
#include <SDL.h>
#include "SDL_image.h"
#include "sprite.h"


enum EntityType
{
	PLAYER,
	ENEMY,
	BOSS,
	ITEM,
	ENTITY,
};

/**
 * @brief	The entity structure
 */
typedef struct Entity_S
{
	int inuse;
	EntityType type;
	Vec2d pos;
	Vec2d vel;
	Sprite *sprite;
	SDL_Rect bounds;
	int frame;
	float health;
	int maxHealth, speed, strength;
	int nextThink;
	int thinkRate;
	void (*draw)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
	void (*think)(struct Entity_S *self);
	void (*update)(struct Entity_S *self);
	void (*touch)(struct Entity_S *self);
	void (*free)(struct Entity_S **self);

}Entity;


/**
 * @brief	Return a pointer to an empty entity structure
 * @return	Null if it fails, or no more space for entity, else an Entity*.
 */
Entity *Entity_New(char file[], int fw, int fh, Vec2d p);

/**
 * @brief	Frees the memory allocated by the entity.
 * @param	*ent	If not null, a pointer to an entity pointer.
 */
void Entity_Free(Entity **ent);

/**
 * @brief	Entity close system.
 * @param	*ent	If not null, the entity.
 */
void Entity_CloseSystem();

/**
 * @brief	Entity initialise system.
 * @param	ent_Max	Maximum amount of entities system supports.
 */
void Entity_InitSystem(Uint32 ent_Max);

/**   
 * @brief	The Think function for all entities. 
 */
void Entity_ThinkAll();

/**
 * @brief	Entity draw.
 * @param	*ent				If not null, the entity.
 * @param	frame				The frame in the spritesheet thats being drawn.
 * @param	*renderer			The renderer being drawn to.
 * @param	drawPos				The position on the screen the entity is being drawn on.
 */
void Entity_DrawAll();

/**   
 * @brief	The Update function that updates all entities every certain amount of time.  
 */
void Entity_UpdateAll();

void Entity_IntersectAll(Entity *a);

int Entity_Intersect(Entity *a, Entity *b);
#endif