#ifndef __PICK_UPS_H__
#define __PICK_UPS_H__

#include "Entity.h"

void Pickup_Spawn(Entity *pickup);

void Pickup_Update(Entity *pickup);
void Pickup_Think(Entity *pickup);

Entity *Pickup_Heart_New(Vec2d pos);
Entity *Pickup_TempHeart_New(Vec2d pos);
Entity *Boulder_New(Vec2d pos);

void Pickup_Heart_Touch(Entity *heart, Entity *other);
void Pickup_TempHeart_Touch(Entity *tempHeart, Entity *other);
void Boulder_Touch(Entity *boulder, Entity *other);
#endif