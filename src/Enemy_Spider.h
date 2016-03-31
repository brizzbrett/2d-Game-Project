#ifndef __ENEMY_SPIDER_H__
#define __ENEMY_SPIDER_H__

#include "Entity.h"
#include "Enemy_Fire.h"

/**
 * @brief	Loads the spider at specified position.
 *
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 *
 * @return	null if it fails, else a Entity*.
 */

Entity *Spider_Load(int x, int y);

/**
 * @brief	Entity think.
 *
 * @param	*Entity		If non-null, the Entity.
 *
 */
void Spider_Think(Entity *spider);

/**
 * @brief	Entity update.
 *
 * @param	*Entity		If non-null, the Entity.
 */
void Spider_Update(Entity *spider);

/**
 * @brief	Entity touch.
 *
 * @param	*Entity		If non-null, the Entity.
 */
void Spider_Touch(Entity *spider, Entity *other);

#endif