#ifndef __ENEMY_NIGHTMARE_H__
#define __ENEMY_NIGHTMARE_H__

#include "Entity.h"

/**
 * @brief	Nightmare Entity load.
 *
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 *
 * @return	The Nightmare Entity, if not null.
 */
Entity *Nightmare_Load(int x, int y, int levelin);

/**
 * @brief	Nightmare Entity phase 1 think.
 *
 * @param nightmare	The nightmare entity that is thinking.
 */
void Nightmare_PhaseOne(Entity *nightmare);

/**
 * @brief	Nightmare Entity phase 2 think.
 *
 * @param nightmare	The nightmare entity that is thinking.
 */
void Nightmare_PhaseTwo(Entity *nightmare);

/**
 * @brief	Nightmare Entity Update.
 *
 * @param nightmare	The nightmare entity that is updating.
 */
void Nightmare_Update(Entity *nightmare);

/**
 * @brief	Nightmare Entity Update.
 *
 * @param nightmare	The nightmare entity that is being touched.
 * @param other	The entity that is touching.
 */
void Nightmare_Touch(Entity *nightmare, Entity *other);

#endif