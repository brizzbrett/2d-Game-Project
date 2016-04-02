#ifndef __ENEMY_FIRE_H__
#define __ENEMY_FIRE_H__

#include "Entity.h"

/**
 * @brief	Creates the enemy bullet.
 *
 * @param owner	The entity that the bullet came from.
 * @param vel	The velocity of the bullet.
 */
void Weapon_Fire(Entity *owner, Vec2d vel);

/**
 * @brief	Weapon Entity think.
 *
 * @param shot	The bullet entity that is thinking.
 */
void Weapon_Think(Entity *shot);

/**
 * @brief	Weapon Entity update.
 *
 * @param shot	The bullet entity that is updating.
 */
void Weapon_Update(Entity *shot);

/**
 * @brief	Weapon Entity touch.
 *
 * @param shot	The bullet entity that is being touched.
 * @param other	The entity that is touching.
 */
void Weapon_Touch(Entity *shot, Entity *other);

#endif