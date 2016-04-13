#include "Enemy_Fire.h"
#include "Camera.h"

Vec2d velocity;
Vec2d pPos;
int offsetX = 0;
int offsetY = 0;
void Weapon_Fire(Entity *owner, Vec2d v)
{	
	Entity *shot;	

	Vec2d pos, vel;
	vec2d_Set(pos, owner->pos.x + offsetX, owner->pos.y + offsetY);
	if(!owner)
	{
		return;
	}
	shot = Entity_New(SHOT, pos); 
	if(!shot)
		return;
	shot->think = &Weapon_Think;
	shot->update = &Weapon_Update;
	shot->touch = &Weapon_Touch;

	shot->nextThink = SDL_GetTicks() + shot->thinkRate;

	vec2d_Set(vel, v.x, v.y);
	shot->vel = vel;

	shot->target = Entity_GetByType(PLAYER);
	shot->owner = owner; 

	pPos = shot->target->pos;	
	vec2d_Subtract(pPos,shot->pos,shot->direction);
	vec2d_Normalize(&shot->direction);	
	vec2d_Multiply(shot->vel,shot->direction,shot->vel);
}

void Weapon_Think(Entity *shot)
{
	if(!shot)
	{
		return;
	}
	shot->nextThink = SDL_GetTicks() + shot->thinkRate;
	if(!Camera_Intersect(shot))
	{
		Entity_Free(&shot);
	}
	if(shot && SDL_GetTicks() >= shot->nextThink)
	{	
		Entity_Free(&shot);	
	}
	
}
void Weapon_Update(Entity *shot)
{
	if(!shot)
	{
		return;
	}	
	if(!shot->owner->inuse)
	{
		Entity_Free(&shot);
		return;
	}	
	shot->target = Entity_GetByType(PLAYER);
	vec2d_Add(shot->pos, shot->vel, shot->pos);


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