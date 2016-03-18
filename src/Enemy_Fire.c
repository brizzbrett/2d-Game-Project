#include "Enemy_Fire.h"
#include "Camera.h"

Vec2d velocity;
Vec2d pPos;
int offsetX = 0;
int offsetY = 0;
void Weapon_Fire(Entity *entity, Vec2d v)
{	

	Shot *shot;	

	Vec2d pos, vel;
	vec2d_Set(pos, entity->pos.x + offsetX, entity->pos.y + offsetY);
	if(!entity)
	{
		return;
	}
	shot = Entity_New("images/shot.png", 100, 100, pos); 
	if(!shot)
		return;
	shot->think = &Weapon_Think;
	shot->update = &Weapon_Update;
	shot->touch = &Weapon_Touch;

	shot->bounds = rect(25,25,25,25);

	shot->nextThink = 0;
	shot->thinkRate = 1500;

	vec2d_Set(vel, v.x, v.y);
	shot->vel = vel;

	shot->target = Entity_GetByID(0);
	shot->owner = entity; //the entity firing owns this projectile

	pPos = shot->target->pos;	
	vec2d_Subtract(pPos,shot->pos,shot->direction);
	vec2d_Normalize(&shot->direction);	
	vec2d_Multiply(shot->vel,shot->direction,shot->vel);
}

void Weapon_Think(Shot *shot)
{
	if(!shot)
	{
		return;
	}
	shot->nextThink = SDL_GetTicks() + shot->thinkRate;
	if(!Camera_Intersect(Camera_GetActiveCamera(),shot))
	{
		Entity_Free(&shot);
	}
	if(shot && SDL_GetTicks() >= shot->nextThink)
	{	
		Entity_Free(&shot);	
	}
	
}
void Weapon_Update(Shot *shot)
{
	if(!shot)
	{
		return;
	}	
	if(!shot->owner)
	{
		Entity_Free(&shot);
	}
	vec2d_Add(shot->pos, shot->vel, shot->pos);
	if(SDL_GetTicks() >= shot->owner->nextThink)
	{
		Entity_Free(&shot);
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
}