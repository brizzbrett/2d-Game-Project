#include "Enemy_Fire.h"
#include "Camera.h"
	
Vec2d direction;
Vec2d velocity;
Vec2d pPos;
int offsetX = 0;
int offsetY = 0;
void Weapon_Fire(Entity *entity)
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
	vec2d_Set(vel, 5, 5);
	shot->vel = vel;

	shot->target = Entity_GetByID(0);
	shot->owner = entity; //the entity firing owns this projectile

	pPos = shot->target->pos;	
	vec2d_Subtract(pPos,shot->pos,direction);
	vec2d_Normalize(&direction);	
	vec2d_Multiply(shot->vel,direction,velocity);
}

void Weapon_Think(Entity *shot)
{
	if(!shot)
	{
		return;
	}

	if(!Camera_Intersect(Camera_GetActiveCamera(),shot))
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
	vec2d_Add(shot->pos, velocity, shot->pos);
	if(SDL_GetTicks() >= shot->owner->nextThink)
	{
		Entity_Free(&shot);
	}
}

void Weapon_Touch(Entity *shot)
{
	Vec2d force;
	vec2d_Set(force,1,1);
	shot->target->health -= .5;
	vec2d_Multiply(velocity, force, velocity);
	vec2d_Add(shot->target->pos,velocity,shot->target->pos);
	Entity_Free(&shot);
}