#ifndef __ENEMY_SPIDER_H__
#define __ENEMY_SPIDER_H__

#include "Entity.h"
#include "Enemy_Fire.h"

/**
 * @brief	Spider Entity Load.
 *
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 *
 * @return	The spider entity, if not null.
 */

Entity *Spider_Load(int x, int y);


/**
 * @brief	Spider Entity think.
 *
 * @param spider	The spider entity that is thinking.
 */
void Spider_Think(Entity *spider);

/**
 * @brief	Spider Entity update.
 *
 * @param spider	The spider entity that is updating.
 */
void Spider_Update(Entity *spider);

/**
 * @brief	Spider Entity touch.
 *
 * @param spider	The spider entity that is being touched.
 * @param other		The entity that is touching.
 */
void Spider_Touch(Entity *spider, Entity *other);

#endif