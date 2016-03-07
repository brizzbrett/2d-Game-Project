#ifndef __ENEMY_FIRE_H__
#define __ENEMY_FIRE_H__

#include "Entity.h"
#include "Vector.h"
#include "simple_logger.h"

typedef Entity Shot;

void Weapon_Fire(Entity *entity);

void Weapon_Think(Entity *spice);

void Weapon_Free(Entity *spice);

void Weapon_Update(Entity *spice);

void Weapon_Touch(Entity *spice);

#endif