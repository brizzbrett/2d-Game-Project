#include "Enemy_Spider.h"
#include "Pick_Ups.h"
#include "Camera.h"

#include <stdlib.h>
#include <math.h>
#include <random>
#include <time.h>

/**
 * @brief	Entity load.
 *
 * @return	null if it fails, else a Entity*.
 */
Entity *Spider_Load(int x, int y)
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

		spider->type = SPIDER;
		//spider->bounds = rect(25, 25, spider->sprite->frameSize.x-50,spider->sprite->frameSize.y-50);
		spider->strength = 3;
		spider->speed = 2;
		spider->health = 4;
		spider->maxHealth = 4;
		vec2d_Set(spider->vel,3,3);
		spider->thinkRate = 1500;
		spider->fireRate = 5500;
		spider->nextThink = 0;
		spider->nextFire = 0;

		spider->owner = NULL;
		spider->target = Entity_GetByType(PLAYER);
		return spider;
	}
	return NULL;
}

/**
 * @brief	Entity think.
 *
 * @param [in,out]	spider	If non-null, the spider.
 */
void Spider_Think(Entity *spider)
{	
	Vec2d vel;
	vec2d_Set(vel,1,1);
	int randX;
	int randY;

	
	spider->target = Entity_GetByType(PLAYER);
	if(SDL_GetTicks() >= spider->nextThink)
	{
		srand(time(NULL));
		randX = rand() % 2;
		randY = rand() % 2;
		if(randX == 0)
			spider->direction.x = -1;
		else
			spider->direction.x = 1;
		if(randY == 0)
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
		Weapon_Fire(spider, vel);
		spider->nextFire = SDL_GetTicks() + spider->fireRate;
	}
}

/**
 * @brief	Entity update.
 *
 * @param [in,out]	spider	If non-null, the spider.
 */
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
	/*if(spider->pos.x <= Camera_GetPosition().x + 100 || spider->pos.x >= Camera_GetSize().x - 200)
	{
		spider->direction.x = -spider->direction.x;
	}
	if(spider->pos.y <= Camera_GetPosition().y + 100 || spider->pos.y >= Camera_GetSize().y - 200)
	{
		spider->direction.y = -spider->direction.y;
	}*/
	if(rect_intersect(rect(spider->pos.x+25, spider->pos.y+25,75,75), spider->target->attack))
	{
		itemPick = rand() % 30;
		finalPos = spider->pos;
		Entity_Free(&spider);
		if(itemPick % 3 == 0)
			Pickup_Spawn(Pickup_Heart_New(finalPos));
		else if(itemPick % 5 == 0)
			Pickup_Spawn(Pickup_TempHeart_New(finalPos));
		else
			Pickup_Spawn(NULL);
	}
	if(Camera_Intersect(spider))
	{
		spider->think = &Spider_Think;
		spider->flag = 1;
	}
	else
	{
		if(!spider)return;
		spider->think = NULL;
		spider->flag = 0;
	}
	Entity_IntersectAll(spider);
}

/**
 * @brief	Entity touch.
 *
 * @param [in,out]	spider	If non-null, the spider.
 */

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