#ifndef __ENEMY_FIRE_H__
#define __ENEMY_FIRE_H__

#include "Entity.h"

void Weapon_Fire(Entity *owner, Vec2d vel);
void Weapon_Think(Entity *shot);
void Weapon_Free(Entity *shot);
void Weapon_Update(Entity *shot);
void Weapon_Touch(Entity *shot, Entity *other);

#endif