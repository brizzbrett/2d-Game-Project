#ifndef __ENEMY_GLOP_H__
#define __ENEMY_GLOP_H__

#include "Entity.h"

typedef Entity Glop;


Glop *Glop_Load();
void Glop_Think(Entity *ent);
void Glop_Update(Entity *ent);
void Glop_Touch(Entity *ent);
void Glop_Attack();

#endif