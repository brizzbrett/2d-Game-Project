#include "Pick_Ups.h"
#include "Camera.h"

void Pickup_Spawn(Entity *pickup)
{
	if(!pickup) return;

	pickup->update = &Pickup_Update;
	pickup->think = NULL;
	pickup->target = Entity_GetByType(PLAYER);

	pickup->nextThink = SDL_GetTicks() + 8000;
	pickup->thinkRate = 0;
}

Entity *Pickup_Heart_New(Vec2d pos)
{
	Entity *heart;
	heart = Entity_New(PICKUP_HEART, pos);
	if(!heart)return NULL;
	heart->touch = &Pickup_Heart_Touch;

	return heart;
}

Entity *Pickup_TempHeart_New(Vec2d pos)
{
	Entity *tempHeart;
	tempHeart = Entity_New(PICKUP_TEMPHEART, pos);
	if(!tempHeart)return NULL;
	tempHeart->touch = &Pickup_TempHeart_Touch;

	return tempHeart;
}
Entity *Boulder_New(Vec2d pos)
{
	Entity *boulder;
	boulder = Entity_New(BOULDER, pos);
	if(!boulder)return NULL;
	boulder->touch = &Boulder_Touch;

	return boulder;
}
void Pickup_Update(Entity *pickup)
{
	pickup->target = Entity_GetByType(PLAYER);
	Entity_IntersectAll(pickup);
	if(Camera_Intersect(pickup) && pickup->type != BOULDER)
	{
		pickup->think = &Pickup_Think;
	}
	else
	{
		pickup->think = NULL;
	}
}

void Pickup_Think(Entity *pickup)
{
	if(!pickup)return;

	if(SDL_GetTicks() >= pickup->nextThink)
	{
		Entity_Free(&pickup);
	}
}
void Pickup_Heart_Touch(Entity *heart, Entity *other)
{
	if(other == heart->target)
	{
		if(other->maxHealth - other->health >= 1)
			other->health += 1;
		else
			other->health = other->maxHealth;
		Entity_Free(&heart);
	}
}

void Pickup_TempHeart_Touch(Entity *tempHeart, Entity *other)
{
	if(other == tempHeart->target)
	{
		other->health += 1;
		Entity_Free(&tempHeart);
	}
}
void Boulder_Touch(Entity *boulder, Entity *other)
{

	if(other == boulder->target)
	{
		other->vel.x = other->vel.x * .5;
		other->vel.y = other->vel.y * .5;
		vec2d_Add(other->vel,other->vel,boulder->vel);
		vec2d_Add(boulder->vel, boulder->pos, boulder->pos);
	}
	else
	{
		vec2d_Set(other->vel,0,0);
	}
}