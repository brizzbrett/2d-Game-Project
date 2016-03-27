#ifndef __ENEMY_GLOP_H__
#define __ENEMY_GLOP_H__

#include "Entity.h"

typedef Entity Entity;

/**
 * @brief	Entity load.
 *
 * @return	null if it fails, else a Entity*.
 */
Entity *Glop_Load(int x, int y);

/**
 * @brief	Entity think.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */
void Glop_Think(Entity *glop);

/**
 * @brief	Entity update.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */
void Glop_Update(Entity *glop);

/**
 * @brief	Entity touch.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */
void Glop_Touch(Entity *glop, Entity *other);

#endif