#include "Items.h"
#include "Camera.h"
#include "Level.h"

bool inDream = FALSE;
Entity *hub;
void Item_Spawn(Entity *item, int levelin)
{
	if(!item) return;

	item->think = NULL;
	item->target = Entity_GetByType(PLAYER);
	item->levelin = levelin;
	item->nextThink = SDL_GetTicks() + 8000;
	item->thinkRate = 0;
}

Entity *Pickup_Heart_New(Vec2d pos)
{
	Entity *heart;
	heart = Entity_New(PICKUP_HEART, pos);
	if(!heart)return NULL;
	heart->touch = &Pickup_Heart_Touch;
	heart->update = &Pickup_Update;
	return heart;
}

Entity *Pickup_TempHeart_New(Vec2d pos)
{
	Entity *tempHeart;
	tempHeart = Entity_New(PICKUP_TEMPHEART, pos);
	if(!tempHeart)return NULL;
	tempHeart->touch = &Pickup_TempHeart_Touch;
	tempHeart->update = &Pickup_Update;
	return tempHeart;
}
Entity *Boulder_New(Vec2d pos)
{
	Entity *boulder;
	boulder = Entity_New(BOULDER, pos);
	if(!boulder)return NULL;
	boulder->touch = &Boulder_Touch;
	boulder->update = &Item_Update;
	return boulder;
}
Entity *Bed_New(Vec2d pos, int bedlvl)
{
	Entity *bed;

	bed = Entity_New(BED, pos);
	if(!bed)return NULL;
	bed->touch = &Bed_Touch;
	bed->bedLevel = bedlvl;
	bed->update = &Item_Update;
	return bed;
}

Entity *Key_New(Vec2d pos, int color)
{
	Entity *key;
	key = Entity_New(KEY, pos);
	if(!key)return NULL;
	key->touch = &Key_Touch;
	key->frame = color;
	key->update = &Item_Update;
	return key;
}

void Pickup_Update(Entity *pickup)
{
	pickup->target = Entity_GetByType(PLAYER);
	Entity_IntersectAll(pickup);

	if(Camera_Intersect(pickup))
	{
		pickup->think = &Pickup_Think;
	}
	else
	{
		pickup->think = NULL;
	}
}

void Item_Update(Entity *item)
{
	item->target = Entity_GetByType(PLAYER);
	Entity_IntersectAll(item);
	/*if(item->type == BED)
	{
		if(
	}*/
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
			other->health = other->health;
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
		if(bed->bedLevel == 1)
		{
			Level_Load(1, 0);
			bed->touch = NULL;
		}
		if(bed->bedLevel == 2)
		{
			Level_Load(2, 0);
			bed->touch = NULL;
		}
		if(bed->bedLevel == 3)
		{
			Level_Load(3, 0);
			bed->touch = NULL;
		}
		if(bed->bedLevel == 4)
		{
			Level_Load(4, 0);
			bed->touch = NULL;
		}
		other->bedPos = bed->pos;
		hub = bed;
		inDream = TRUE;
		
	}
	else if(other == bed->target && inDream)
	{
		other->pos.y = other->bedPos.y + 80;
		other->pos.x = other->bedPos.x + 50;
		other->set = FALSE;
		vec2d_Set(temp, hub->pos.x-150, hub->pos.y-400);
		hub->flag = 1;
		Camera_SetPosition(temp);
		inDream = FALSE;
		Level_Save();
		Level_Closer(hub->bedLevel);
	}
	else
	{
		vec2d_Set(other->vel,0.1,0.1);
	}
}
void Key_Touch(Entity *key, Entity *other)
{
	if(other == key->target)
	{
		key->target->keys++;
		slog("Player now has: %i keys", key->target->keys);
		Entity_Free(&key);
	}
}