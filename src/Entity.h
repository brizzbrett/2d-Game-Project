#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Vector.h"
#include "Graphics.h"
#include <SDL.h>
#include "SDL_image.h"
#include "Sprite.h"
#include "Audio.h"
#include "simple_logger.h"
#include "cJSON.h"
#include <glib.h>

/**
 * @brief	EntityType enum
 */
enum EntityType
{
	PLAYER,				/**<Player entities */
	GLOP,				/**<Glop entities */
	EYE,				/**<Eye entities */
	SPIDER,				/**<Spider entities */
	SHOT,				/**<Bullet entities */
	BOSS,				/**<Boss entities */
	PICKUP_HEART,		/**<Heart pickup entities */
	PICKUP_TEMPHEART,	/**<Temporary Heart pickup entities */
	BOULDER,			/**<Boulder entities */
	BED,				/**<Bed entities */
	SDOOR,				/**<South door entities */
	NDOOR,				/**<North door entities */
	EDOOR,				/**<East door entities */
	WDOOR,				/**<West door entities */
	OTHER,				/**<Any other entity that I did not catch */
};

/**
 * @brief	Entity structure for all entities
 */
typedef struct Entity_S
{
	int inuse;			/**<Entity inuse flag */
	EntityType type;	/**<The entities type */
	int id;				/**<The entities id */

	Vec2d pos;			/**<Position in vector 2d */
	Vec2d levelPos;
	Vec2d vel;			/**<Velocity in vector 2d */
	Vec2d velocity9;	/**<Manipulated Velocity in vector 2d */
	Vec2d direction;	/**<Direction in vector 2d */
	Vec2d force;		/**<Forec in vector 2d */

	Sprite *sprite;		/**<Entity sprite info */
	Sound *sound;

	SDL_Rect bounds;	/**<The collision bounds */
	SDL_Rect attack;	/**<The attacking bounds (for player) */

	int frame;			/**<The current frame of the sprite */

	float health;		/**<Entity health, if have */
	int maxHealth, speed, strength;	/**<Entity max health, speed and strength, if have */

	int nextThink;		/**<The next think for an entity */
	int thinkRate;		/**<The rate of thinking */
	int nextFire;		/**<The next fire for an entity that fires bullets */
	int fireRate;		/**<The rate of fire */

	int flag;			/**<Misc flag usage */

	bool set;

	int levelin;

	int bedLevel;

	struct Entity_S *owner;		/**<The Entity that owns this entity, if it has one */
	struct Entity_S *target;	/**<The Entity that this entity is targeting */

	void (*draw)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);	/**<Draw function pointer */
	void (*think)(struct Entity_S *self);										/**<Think function pointer */
	void (*update)(struct Entity_S *self);										/**<Update function pointer */
	void (*touch)(struct Entity_S *self, struct Entity_S *other);				/**<Touch function pointer */

}Entity;

/**
 * @brief	Return an Entity* and adds it to the entity list
 * 			
 * @param EntityType	the type of entity being created
 * @param pos			Position the entity is placed in the world
 * 						
 * @return	Null if it fails, or no more space for entity list, else an Entity*.
 */
Entity *Entity_New(EntityType type, Vec2d pos);
/**
 * @brief	Frees the memory allocated by the entity.
 * 			
 * @param	*ent	If not null, a pointer to an entity pointer.
 */
void Entity_Free(Entity **ent);

/**
 * @brief	Entity close system.
 * @param	*ent	If not null, the entity.
 */
void Entity_CloseSystem();

void Entity_FreeByLevel(int level);
/**
 * @brief	Entity initialise system.
 * 			
 * @param	ent_Max	Maximum amount of entities system supports.
 */
void Entity_InitSystem(Uint32 ent_Max);

/**   
 * @brief	The Think function for all entities. 
 */
void Entity_ThinkAll();

/**
 * @brief	Entity draw function for all entities.
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

GList *EntList_Get();
#endif