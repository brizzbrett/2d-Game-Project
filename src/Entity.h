#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Vector.h"
#include "Graphics.h"
#include <SDL.h>
#include "SDL_image.h"
#include "Sprite.h"
#include "simple_logger.h"
#include "cJSON.h"

/**
 * @brief	Boolean type enum
 */
enum BrettBool
{
	FALSE,
	TRUE
};
/**
 * @brief	EntityType enum
 */
enum EntityType
{
	PLAYER,
	GLOP,
	EYE,
	SPIDER,
	SHOT,
	BOSS,
	PICKUP_HEART,
	PICKUP_TEMPHEART,
	BOULDER,
	SDOOR,
	NDOOR,
	EDOOR,
	WDOOR,
	OTHER,
};

/**
 * @brief	Entity structure for all entities
 */
typedef struct Entity_S
{
	int inuse;
	EntityType type;
	int id;

	Vec2d pos;
	Vec2d vel;
	Vec2d velocity9;
	Vec2d direction;
	Vec2d force;
	Vec2d offsetNW;
	Vec2d offsetSE;

	Sprite *sprite;

	SDL_Rect bounds;
	SDL_Rect attack;

	int frame;

	float health;
	int maxHealth, speed, strength;

	int nextThink;
	int thinkRate;
	int nextFire;
	int fireRate;

	int flag;

	struct Entity_S *owner;
	struct Entity_S *target;

	void (*draw)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
	void (*think)(struct Entity_S *self);
	void (*update)(struct Entity_S *self);
	void (*touch)(struct Entity_S *self, struct Entity_S *other);

}Entity;


/**
 * @brief	Return a pointer to an empty entity structure
 * @return	Null if it fails, or no more space for entity, else an Entity*.
 */
Entity *Entity_New(EntityType type, Vec2d pos);
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
 * @brief	The Update function that updates all entities every frame.  
 */
void Entity_UpdateAll();

/**   
 * @brief	The Intersect/Touch function that checks all entities for collision every frame.  
 *
 * @param	The entity being checked against the rest.
 */
void Entity_IntersectAll(Entity *a);
/**   
 * @brief	The Intersect function that checks if two rects are colliding.  
 *
 * @param	*a	The first entity
 * @param	*b	The second entity
 *
 * @return	0 or 1 depending on whether or not the rects collided.
 */
int Entity_Intersect(Entity *a, Entity *b);

/**   
 * @brief	Function that is used to get an Entity by it's ID.  
 *
 * @param	id	The id of the entity you're looking for.
 *
 * @return	The entity with the id specified.
 */
Entity *Entity_GetByID(int id);

/**   
 * @brief	Function that is used to get an Entity by it's Type.
 *
 * @param	The type of entity you're looking for.
 *
 * @return The entity with the type specified.
 */
Entity *Entity_GetByType(EntityType type);

/**   
 * @brief	Get's the number of entities currently in the level.  
 *
 * @return	The number of entities.
 */
int Entity_GetNum();


#endif