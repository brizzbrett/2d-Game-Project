#ifndef __ENEMY_GLOP_H__
#define __ENEMY_GLOP_H__

#include "Entity.h"

/**
 * @brief	Glop Entity load.
 *
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 *
 * @return	The Glop Entity, if not null.
 */
Entity *Glop_Load(int x, int y, int levelin);

/**
 * @brief	Glop Entity think.
 *
 * @param glop	The glop entity that is thinking.
 */
void Glop_Think(Entity *glop);

/**
 * @brief	Glop Entity Update.
 *
 * @param glop	The glop entity that is updating.
 */
void Glop_Update(Entity *glop);

/**
 * @brief	Glop Entity Update.
 *
 * @param glop	The glop entity that is being touched.
 * @param other	The entity that is touching.
 */
void Glop_Touch(Entity *glop, Entity *other);

#endif