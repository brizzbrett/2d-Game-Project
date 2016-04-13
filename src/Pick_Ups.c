#include "Pick_Ups.h"
#include "Camera.h"
#include "Level.h"

bool inDream = FALSE;
Entity *hub;
void Item_Spawn(Entity *item)
{
	if(!item) return;

	item->update = &Item_Update;
	item->think = NULL;
	item->target = Entity_GetByType(PLAYER);

	item->nextThink = SDL_GetTicks() + 8000;
	item->thinkRate = 0;
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
Entity *Bed_New(Vec2d pos)
{
	Entity *bed;
	bed = Entity_New(BED, pos);
	if(!bed)return NULL;
	bed->touch = &Bed_Touch;

	return bed;
}

void Item_Update(Entity *item)
{
	item->target = Entity_GetByType(PLAYER);
	Entity_IntersectAll(item);
	if(Camera_Intersect(item) && item->type != BOULDER && item->type != BED)
	{
		item->think = &Pickup_Think;
	}
	else
	{
		item->think = NULL;
	}
}

void Pickup_Think(Entity *item)
{
	if(!item)return;

	if(SDL_GetTicks() >= item->nextThink)
	{
		Entity_Free(&item);
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
void Bed_Touch(Entity *bed, Entity *other)
{
	Vec2d temp;
	if(other == bed->target && !inDream)
	{
		Level_Load(1);
		//vec2d_Set(temp, bed->pos.x-200, bed->pos.y-450);
		hub = bed;
		inDream = TRUE;
	}
	else if(other == bed->target && inDream)
	{
		other->pos.y = hub->pos.y + 170;
		other->pos.x = hub->pos.x + 50;
		vec2d_Set(temp, hub->pos.x-200, hub->pos.y-450);
		Camera_SetPosition(temp);
		inDream = FALSE;
	}
	else
	{
		vec2d_Set(other->vel,0,0);
	}
}