#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Vector.h"
typedef Entity Player;

Player *Player_Load(int x, int y);
Player *Player_Get();
void Player_Think(Player *player);
void Player_Update(Player * player);
void Player_Touch(Player *player);

#endif