#ifndef __PICK_UPS_H__
#define __PICK_UPS_H__

#include "Entity.h"

typedef Entity Pickup;

void Pickup_Spawn(Pickup *pickup);

void Pickup_Update(Pickup *pickup);
void Pickup_Think(Pickup *pickup);

Pickup *Pickup_Heart_New(Vec2d pos);
Pickup *Pickup_TempHeart_New(Vec2d pos);

void Pickup_Heart_Touch(Pickup *heart, Entity *other);
void Pickup_TempHeart_Touch(Entity *tempHeart, Entity *other);

#endif