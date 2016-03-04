#ifndef __SAVE_H__
#define __SAVE_H__
#include "Vector.h"

typedef struct
{
	Vec2d pos;
	Vec2d vel;
	int frame;
	float health;
	int maxHealth, speed, strength;
	int eventNum;
	char levelName[64];
}PlayerSave;

typedef struct
{
	char level[64];
	char entity[32];
	char status[32];
}Events;

#endif