#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

typedef Entity Player;

Player *Player_Load();
Player *Player_Get();
void Player_Think(Entity *ent);
void Player_Update(Entity *ent);
void Player_Touch(Entity *ent);
int You_Died();

#endif