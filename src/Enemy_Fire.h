#ifndef __ENEMY_FIRE_H__
#define __ENEMY_FIRE_H__

#include "Entity.h"
#include "Vector.h"
#include "simple_logger.h"

typedef Entity Shot;

void Weapon_Fire(Entity *owner, Vec2d vel);
void Weapon_Think(Shot *shot);
void Weapon_Free(Shot *shot);
void Weapon_Update(Shot *shot);
void Weapon_Touch(Shot *shot, Entity *other);

#endif