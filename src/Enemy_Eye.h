#ifndef __ENEMY_EYE_H__
#define __ENEMY_EYE_H__

#include "Entity.h"
#include "Enemy_Fire.h"

typedef Entity Eye;

/**
 * @brief	Eye load.
 *
 * @return	null if it fails, else a Eye*.
 */
Eye *Eye_Load(int x, int y);

/**
 * @brief	Eye think.
 *
 * @param [in,out]	Eye	If non-null, the Eye.
 */
void Eye_Think(Eye *Eye);

/**
 * @brief	Eye update.
 *
 * @param [in,out]	Eye	If non-null, the Eye.
 */
void Eye_Update(Eye *Eye);

/**
 * @brief	Eye touch.
 *
 * @param [in,out]	Eye	If non-null, the Eye.
 */
void Eye_Touch(Eye *Eye, Entity *other);

#endif