#ifndef __ENEMY_EYE_H__
#define __ENEMY_EYE_H__

#include "Entity.h"
#include "Enemy_Fire.h"

/**
 * @brief	Entity load.
 *
 * @return	null if it fails, else a Entity*.
 */
Entity *Eye_Load(int x, int y);

/**
 * @brief	Entity think.
 *
 * @param [in,out]	Entity	If non-null, the Entity.
 */
void Eye_Think(Entity *eye);

/**
 * @brief	Entity update.
 *
 * @param [in,out]	Entity	If non-null, the Entity.
 */
void Eye_Update(Entity *eye);

/**
 * @brief	Entity touch.
 *
 * @param [in,out]	Entity	If non-null, the Entity.
 */
void Eye_Touch(Entity *eye, Entity *other);

#endif