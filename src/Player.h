#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

Entity *Player_Load(int x, int y);
Entity *Player_Get();
void Player_Think(Entity *player);
void Player_Update(Entity *player);
void Player_Touch(Entity *player, Entity *other);

#endif