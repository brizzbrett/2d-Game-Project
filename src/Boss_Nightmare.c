#include "Boss_Nightmare.h"
#include "Items.h"
#include "Enemy_Fire.h"

#include <stdlib.h>
#include <math.h>
#include "Camera.h"

int keyColor = 0;
Entity *Nightmare_Load(int x, int y, int levelin)
{
	Entity *nightmare;
	Vec2d gPos;
	int i;

	vec2d_Set(gPos,x,y);

	nightmare = Entity_New(NIGHTBOSS, gPos);

	if(nightmare)
	{
		nightmare->think = NULL;
		nightmare->touch = &Nightmare_Touch;
		nightmare->update = &Nightmare_Update;

		vec2d_Set(nightmare->vel,1,1);
		nightmare->nextThink = SDL_GetTicks();
		nightmare->owner = NULL;
		nightmare->target = Entity_GetByType(PLAYER);
		nightmare->flag = 0;
		nightmare->sound = Sound_New("audio/nightmare.ogg",0,ENEMY_FX);
		nightmare->keys = 0;
		vec2d_Set(nightmare->velocity9,3,3);
		for(i = 0; i < 8; i++)
		{
			nightmare->shot[i] = NULL;
		}
		nightmare->levelin = levelin;
		return nightmare;
	}
	return NULL;
}

void Nightmare_PhaseOne(Entity *nightmare)
{	
	Vec2d vel;
	int i;
	vec2d_Set(vel,3,3);

	nightmare->target = Entity_GetByType(PLAYER);
	
	if(SDL_GetTicks() >= nightmare->nextThink)
	{
		nightmare->shot[0] = Weapon_Fire(nightmare, vel, 1, 0, 0);
		vec2d_Negate(nightmare->shot[0]->velocity9,nightmare->shot[0]->vel);

		nightmare->shot[1] = Weapon_Fire(nightmare, vel, -1, 1, 0);
		vec2d_Negate(nightmare->shot[1]->velocity9,nightmare->shot[1]->vel);

		nightmare->shot[2] = Weapon_Fire(nightmare, vel, 1, 1, 0);
		vec2d_Negate(nightmare->shot[2]->velocity9,nightmare->shot[2]->vel);

		nightmare->shot[3] = Weapon_Fire(nightmare, vel, 0, 1, 0);
		vec2d_Negate(nightmare->shot[3]->velocity9,nightmare->shot[3]->vel);

		nightmare->shot[4] = Weapon_Fire(nightmare, vel, -1, 0, 0);
		vec2d_Negate(nightmare->shot[4]->velocity9,nightmare->shot[4]->vel);

		nightmare->shot[5] = Weapon_Fire(nightmare, vel, -1, -1, 0);
		vec2d_Negate(nightmare->shot[5]->velocity9,nightmare->shot[5]->vel);

		nightmare->shot[6] = Weapon_Fire(nightmare, vel, 0, -1, 0);
		vec2d_Negate(nightmare->shot[6]->velocity9,nightmare->shot[6]->vel);

		nightmare->shot[7] = Weapon_Fire(nightmare, vel, 1, -1, 0);	
		vec2d_Negate(nightmare->shot[7]->velocity9,nightmare->shot[7]->vel);

		nightmare->nextThink = SDL_GetTicks() + nightmare->thinkRate+1000;
		nightmare->nextFire = SDL_GetTicks() + nightmare->thinkRate+100;
		nightmare->shot[0]->flag = 1;
		nightmare->shot[1]->flag = 1;
		nightmare->shot[2]->flag = 1;
		nightmare->shot[3]->flag = 1;
		nightmare->shot[4]->flag = 1;
		nightmare->shot[5]->flag = 1;
		nightmare->shot[6]->flag = 1;
		nightmare->shot[7]->flag = 1;
	}
	
	for(i = 0; i < 8; i++)
	{	
		if(SDL_GetTicks() >= nightmare->nextFire)
		{
			vec2d_Set(nightmare->shot[i]->vel,4,4);
			vec2d_Subtract(nightmare->target->pos,nightmare->shot[i]->pos,nightmare->shot[i]->direction);
			vec2d_Normalize(&nightmare->shot[i]->direction);	
			vec2d_Multiply(nightmare->shot[i]->vel,nightmare->shot[i]->direction,nightmare->shot[i]->vel);
		}
	}
	for(i = 0; i < 8; i++)
	{
		if(SDL_GetTicks() <= nightmare->nextFire && nightmare->shot[i]->flag && nightmare->shot[i] && vec2d_GetDistanceSquared(nightmare->shot[i]->pos, nightmare->pos) >= 8000)
		{
			vec2d_Set(nightmare->shot[i]->vel,0,0);
			nightmare->shot[i]->flag = 0;
		}
	}
	
}
void Nightmare_PhaseTwo(Entity *nightmare)
{	
	nightmare->target = Entity_GetByType(PLAYER);
	vec2d_Subtract(nightmare->target->pos,nightmare->pos,nightmare->direction);
	if((nightmare->direction.x <= 250 && nightmare->direction.x >= -250) && (nightmare->direction.y <= 150 && nightmare->direction.y >= -150))
	{
		nightmare->flag = 1;
	}
	if(nightmare->flag)
	{
		vec2d_Normalize(&nightmare->direction);
		vec2d_Multiply(nightmare->vel,nightmare->direction,nightmare->velocity9);
		vec2d_Add(nightmare->pos,nightmare->velocity9,nightmare->pos);
	}
}
void Nightmare_Update(Entity *nightmare)
{
	int itemPick;
	Vec2d finalPos;

	if(!nightmare)return;
	vec2d_Set(nightmare->vel, sin((float)time(NULL)), sin((float)time(NULL)));
	nightmare->target = Entity_GetByType(PLAYER);
	if(rect_intersect(rect(nightmare->pos.x+nightmare->bounds.x, nightmare->pos.y+nightmare->bounds.y,nightmare->bounds.w,nightmare->bounds.h), nightmare->target->attack))
	{
		nightmare->health -= 0.5f;
		if(nightmare->health <= 0.0)
		{
			itemPick = rand() % 30;
			finalPos = nightmare->pos;
			Item_Spawn(Key_New(finalPos, keyColor),0);
			keyColor++;
			Entity_Free(&nightmare);
		}
	}
	if(Camera_Intersect(nightmare))
	{
		nightmare->think = &Nightmare_PhaseOne;
	}
	else
	{
		if(!nightmare) return;
		nightmare->think = NULL;
	}

	if (abs(nightmare->target->pos.y-nightmare->pos.y) > abs(nightmare->target->pos.x-nightmare->pos.x)) 
	{
		if ((nightmare->target->pos.y-nightmare->pos.y) > 0) 
		{
			nightmare->frame = 1;
		} 
		else if ((nightmare->target->pos.y-nightmare->pos.y) < 0) 
		{
			nightmare->frame = 2;
		}
	} 
	else
	{
		if ((nightmare->target->pos.x-nightmare->pos.x) == 0 && (nightmare->target->pos.y-nightmare->pos.y) == 0) 
		{
			nightmare->frame = 1;
		}
		if ((nightmare->target->pos.x-nightmare->pos.x) > 0) 
		{
			nightmare->frame = 0;
		} 
		else if ((nightmare->target->pos.x-nightmare->pos.x) < 0) 
		{
			nightmare->frame = 3;

		}
	}
	Entity_IntersectAll(nightmare);
}

void Nightmare_Touch(Entity *nightmare, Entity *other)
{
	if(other == nightmare->target)
	{

	}
}