#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

typedef Entity Player;

Player *Player_Load();
void Player_Think(Entity *ent);
void Player_Update();
void Player_Attack();

#endif