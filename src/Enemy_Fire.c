#include "Enemy_Fire.h"
#include "Camera.h"

Vec2d velocity;
Vec2d pPos;
int offsetX = 0;
int offsetY = 0;
Entity *Weapon_Fire(Entity *owner, Vec2d v, int dirX, int dirY, int frame)
{	
	Entity *shot;	

	Vec2d pos, vel;
	vec2d_Set(pos, owner->pos.x + offsetX, owner->pos.y + offsetY);
	if(!owner)
	{
		return NULL;
	}
	shot = Entity_New(SHOT, pos); 
	if(!shot)
		return NULL;
	shot->think = &Weapon_Think;
	shot->update = &Weapon_Update;
	shot->touch = &Weapon_Touch;
	shot->frame = 3;
	shot->nextThink = SDL_GetTicks() + shot->thinkRate;

	vec2d_Set(vel, v.x, v.y);
	shot->vel = vel;

	shot->target = Entity_GetByType(PLAYER);
	shot->owner = owner; 

	if(shot->owner->type == NIGHTBOSS)
	{
		shot->direction.x = dirX;
		shot->direction.y = dirY;
		vec2d_Multiply(shot->vel, shot->direction, shot->vel);
	}
	else
	{
		Weapon_TargetPlayer(shot);
	}


	return shot;
}
void Weapon_TargetPlayer(Entity *shot)
{
	pPos = shot->target->pos;
	vec2d_Subtract(pPos,shot->pos,shot->direction);
	vec2d_Normalize(&shot->direction);	
	vec2d_Multiply(shot->vel,shot->direction,shot->vel);
}
void Weapon_Think(Entity *shot)
{
	int up_or_down = 0;
	if(!shot)
	{
		return;
	}
	if(shot->owner->type == NIGHTBOSS)
	{
		if(shot->frame == 1)
		{
			up_or_down = 1;
		}
		else if(shot->frame == 3)
		{
			up_or_down = 0;
		}
		if(up_or_down == 1 && SDL_GetTicks() >= shot->nextThink)
		{
			shot->frame++;
			shot->nextThink = SDL_GetTicks() + 100;
		}
		else if(up_or_down == 0 && SDL_GetTicks() >= shot->nextThink)
		{
			shot->frame--;
			shot->nextThink = SDL_GetTicks() + 100;
		}
	}
	//shot->nextThink = SDL_GetTicks() + shot->thinkRate;
	if(!Camera_Intersect(shot))
	{
		Entity_Free(&shot);
	}
	
}
void Weapon_Update(Entity *shot)
{
	Vec2d vel;
	vec2d_Set(vel, 3,3);
	if(!shot)
	{
		return;
	}	
	if(!shot->owner->inuse)
	{
		Entity_Free(&shot);
		return;
	}	
	//shot->target = Entity_GetByType(PLAYER);
	vec2d_Add(shot->pos, shot->vel, shot->pos);
	if(rect_intersect(rect(shot->pos.x+shot->bounds.x, shot->pos.y+shot->bounds.y,shot->bounds.w,shot->bounds.h), shot->target->attack))
	{
		vec2d_Negate(shot->direction,shot->direction);
		vec2d_Multiply(shot->direction, vel, vel);
		shot->vel = vel;
		shot->target = shot->owner;
		shot->owner = Entity_GetByType(PLAYER);
	}

	Entity_IntersectAll(shot);
}

void Weapon_Touch(Entity *shot, Entity *other)
{
	Vec2d force;
	if(other == shot->target)
	{
		vec2d_Set(force,1,1);
		other->health -= .5;
		vec2d_Multiply(velocity, force, velocity);
		vec2d_Add(other->pos,shot->vel,other->pos);
		Entity_Free(&shot);
	}
	if(other->type == BOULDER)
	{
		Entity_Free(&shot);
	}
}