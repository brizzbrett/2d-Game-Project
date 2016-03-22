#include "Pick_Ups.h"
#include "Camera.h"

void Pickup_Spawn(Pickup *pickup)
{
	if(!pickup) return;

	pickup->draw = &sprite_Draw;
	pickup->update = &Pickup_Update;
	pickup->think = NULL;
	pickup->target = Entity_GetByType(PLAYER);
	if(pickup->type == OTHER)
	{
		pickup->type = PICKUP;
	}

	pickup->nextThink = SDL_GetTicks() + 8000;
	pickup->thinkRate = 0;
}

Pickup *Pickup_Heart_New(Vec2d pos)
{
	Pickup *heart;
	heart = Entity_New("images/heart.png", 100, 100, pos);
	heart->touch = &Pickup_Heart_Touch;

	heart->pos = pos;
	heart->bounds = rect(36,39,63-36,64-39);
	return heart;
}

Pickup *Pickup_TempHeart_New(Vec2d pos)
{
	Pickup *tempHeart;
	tempHeart = Entity_New("images/tempheart.png", 100, 100, pos);
	tempHeart->touch = &Pickup_TempHeart_Touch;

	tempHeart->pos = pos;
	tempHeart->bounds = rect(36,39,63-36,64-39);
	return tempHeart;
}
Pickup *Boulder_New(Vec2d pos)
{
	Pickup *boulder;
	boulder = Entity_New("images/boulder.png", 100, 100, pos);
	boulder->touch = &Boulder_Touch;
	boulder->type = BOULDER;
	boulder->pos = pos;
	boulder->bounds = rect(5,5,100-30,100-10);
	return boulder;
}
void Pickup_Update(Pickup *pickup)
{
	pickup->target = Entity_GetByType(PLAYER);
	Entity_IntersectAll(pickup);
	if(Camera_Intersect(Camera_GetActiveCamera(), pickup) && pickup->type != BOULDER)
	{
		pickup->think = &Pickup_Think;
	}
	else
	{
		pickup->think = NULL;
	}
}

void Pickup_Think(Pickup *pickup)
{
	if(!pickup)return;

	if(SDL_GetTicks() >= pickup->nextThink)
	{
		Entity_Free(&pickup);
	}
}
void Pickup_Heart_Touch(Pickup *heart, Entity *other)
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

void Pickup_TempHeart_Touch(Pickup *tempHeart, Entity *other)
{
	if(other == tempHeart->target)
	{
		other->health += 1;
		Entity_Free(&tempHeart);
	}
}
void Boulder_Touch(Pickup *boulder, Entity *other)
{
	if(other == boulder->target)
	{
		boulder->vel = other->vel;
		vec2d_Add(boulder->vel, boulder->pos, boulder->pos);
	}
	else
	{
		vec2d_Set(other->vel,0,0);
	}
}