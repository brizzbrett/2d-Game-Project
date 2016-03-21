#include "Enemy_Spider.h"
#include "Pick_Ups.h"
#include "Camera.h"

#include <stdlib.h>
#include <math.h>

/**
 * @brief	Spider load.
 *
 * @return	null if it fails, else a Spider*.
 */
Spider *Spider_Load(int x, int y)
{
	Spider *spider;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	spider = Entity_New("images/Spidersheet.png",128,128, gPos);

	if(spider)
	{
		spider->think = NULL;
		spider->touch = &Spider_Touch;
		spider->update = &Spider_Update;

		spider->type = ENEMY;
		spider->bounds = rect(25, 25, spider->sprite->frameSize.x-50,spider->sprite->frameSize.y-50);
		spider->strength = 3;
		spider->speed = 2;
		spider->health = 4;
		spider->maxHealth = 4;
		vec2d_Set(spider->vel,3,3);
		spider->thinkRate = 2500;
		spider->nextThink = 0;

		spider->owner = NULL;
		spider->target = Entity_GetByType(PLAYER);
		return spider;
	}
	return NULL;
}

/**
 * @brief	Spider think.
 *
 * @param [in,out]	spider	If non-null, the spider.
 */
void Spider_Think(Spider *spider)
{	
	Vec2d vel;
	vec2d_Set(vel,0,0);
	int randX = rand() % 3 -1;
	int randY = rand() % 3 -1;

	vec2d_Set(spider->vel, 0.06, 0.06);
	vec2d_Multiply(spider->vel,spider->direction,spider->vel);
	vec2d_Add(spider->pos,spider->vel,spider->pos);

	if(SDL_GetTicks() >= spider->nextThink)
	{
		Weapon_Fire(spider, vel);
		vec2d_Set(spider->direction, (randX), (randY));

		vec2d_Normalize(&spider->direction);
		spider->nextThink = SDL_GetTicks() + spider->thinkRate;	
	}		


}

/**
 * @brief	Spider update.
 *
 * @param [in,out]	spider	If non-null, the spider.
 */
void Spider_Update(Spider *spider)
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
	if(Camera_Intersect(Camera_GetActiveCamera(), spider))
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

/**
 * @brief	Spider touch.
 *
 * @param [in,out]	spider	If non-null, the spider.
 */

void Spider_Touch(Spider *spider, Entity *other)
{
	Vec2d force;
	if(other == spider->target)
	{
		vec2d_Set(force,15,15);
		spider->target->health -= .5;
		vec2d_Multiply(spider->target->vel, force, spider->target->velocity9);
		vec2d_Add(spider->target->pos,-spider->target->velocity9,spider->target->pos);
	}
}