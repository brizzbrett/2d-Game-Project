#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

/**
 * @brief	Player Entity Load.
 *
 * @param x The x position.
 * @param y	The y position.
 *
 * @return	The Player entity being created, if not null.
 */
Entity *Player_Load(int x, int y);

/**
 * @brief	Player entity think.
 *
 * @param player	The player entity that is thinking.
 */
void Player_Think(Entity *player);

/**
 * @brief	Player entity update.
 *
 * @param player	The player entity that is updating.
 */
void Player_Update(Entity *player);

/**
 * @brief	Player entity touch.
 *
 * @param player	The player entity that is being touched.
 * @param other		The entity that is touching.
 */
void Player_Touch(Entity *player, Entity *other);

#endif