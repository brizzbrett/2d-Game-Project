#ifndef __ENEMY_EYE_H__
#define __ENEMY_EYE_H__

#include "Entity.h"
#include "Enemy_Fire.h"

/**
 * @brief	Eye Entity load.
 *
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 *
 * @return	The Eye entity, if not null.
 */
Entity *Eye_Load(int x, int y);

/**
 * @brief	Eye Entity think.
 *
 * @param eye	The entity that is thinking.
 */
void Eye_Think(Entity *eye);

/**
 * @brief	Eye Entity update.
 *
 * @param eye	the entity that is updating.
 */
void Eye_Update(Entity *eye);

/**
 * @brief	Entity touch.
 *
 * @param eye	the entity that is checking for touching.
 * @param other	the entity that is touching.
 */
void Eye_Touch(Entity *eye, Entity *other);

#endif