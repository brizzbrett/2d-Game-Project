#include "Enemy_Spider.h"
#include "Items.h"
#include "Camera.h"

#include <stdlib.h>
#include <math.h>
#include <random>
#include <time.h>

Entity *Spider_Load(int x, int y, int levelin)
{
	Entity *spider;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	spider = Entity_New(SPIDER, gPos);
	
	if(spider)
	{
		spider->think = &Spider_Think;
		spider->touch = &Spider_Touch;
		spider->update = &Spider_Update;

		vec2d_Set(spider->vel,3,3);

		spider->owner = NULL;
		spider->target = Entity_GetByType(PLAYER);

		spider->levelin = levelin;
		return spider;
	}
	return NULL;
}

void Spider_Think(Entity *spider)
{	
	Vec2d vel;
	vec2d_Set(vel,.5,.5);
	int randX;
	int randY;

	
	spider->target = Entity_GetByType(PLAYER);
	if(SDL_GetTicks() >= spider->nextThink)
	{
		srand(time(NULL));
		randX = rand() % 4;
		randY = rand() % 4;
		if(randX % 2 == 0)
			spider->direction.x = -1;
		else
			spider->direction.x = 1;
		if(randY % 2 != 0)
			spider->direction.y = -1;
		else
			spider->direction.y = 1;

		spider->nextThink = SDL_GetTicks() + spider->thinkRate;	
	}
	vec2d_Set(spider->vel, 0.06, 0.06);
	vec2d_Multiply(spider->vel,spider->direction,spider->vel);
	vec2d_Add(spider->pos,spider->vel,spider->pos);

	if(SDL_GetTicks() >= spider->nextFire)
	{
		Weapon_Fire(spider, vel, 0, 0, 0);
		spider->nextFire = SDL_GetTicks() + spider->fireRate;
	}
}

void Spider_Update(Entity *spider)
{
	int itemPick;
	Vec2d finalPos;

	if(!spider)return;
	spider->target = Entity_GetByType(PLAYER);
	if (abs(spider->target->pos.y-spider->pos.y) > abs(spider->target->pos.x-spider->pos.x)) 
	{
		if ((spider->target->pos.y-spider->pos.y) > 0) 
		{
			spider->frame = 0;
		} 
		else if ((spider->target->pos.y-spider->pos.y) < 0) 
		{
			spider->frame = 2;
		}
	} 
	else
	{
		if ((spider->target->pos.x-spider->pos.x) == 0 && (spider->target->pos.y-spider->pos.y) == 0) 
		{
			spider->frame = 0;
		}
		if ((spider->target->pos.x-spider->pos.x) > 0) 
		{
			spider->frame = 3;
		} 
		else if ((spider->target->pos.x-spider->pos.x) < 0) 
		{
			spider->frame = 1;
		}
	}
	if(spider->pos.x <= Camera_GetPosition().x + 100 || spider->pos.x >= Camera_GetSize().x - 200)
	{
		spider->direction.x = -spider->direction.x;
	}
	if(spider->pos.y <= Camera_GetPosition().y + 100 || spider->pos.y >= Camera_GetSize().y - 200)
	{
		spider->direction.y = -spider->direction.y;
	}
	if(rect_intersect(rect(spider->pos.x+spider->bounds.x, spider->pos.y+spider->bounds.y,spider->bounds.w,spider->bounds.h), spider->target->attack))
	{
		itemPick = rand() % 30;
		finalPos = spider->pos;
		Entity_Free(&spider);
		if(itemPick % 3 == 0)
			Item_Spawn(Pickup_Heart_New(finalPos),0);
		else if(itemPick % 5 == 0)
			Item_Spawn(Pickup_TempHeart_New(finalPos),0);
		else
			Item_Spawn(NULL,0);
	}
	if(spider && spider->health <= 0)
	{
		Entity_Free(&spider);
	}
	if(Camera_Intersect(spider))
	{
		spider->think = &Spider_Think;
	}
	else
	{
		if(!spider)return;
		spider->think = NULL;
	}
	Entity_IntersectAll(spider);
}

void Spider_Touch(Entity *spider, Entity *other)
{
	Vec2d force;
	if(other == spider->target)
	{
		vec2d_Set(force,150,150);
		spider->target->health -= .5;
		vec2d_Multiply(spider->vel, force, spider->target->velocity9);
		vec2d_Add(spider->target->pos,spider->target->velocity9,spider->target->pos);
	}
}