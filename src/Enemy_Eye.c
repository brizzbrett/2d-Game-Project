#include "Enemy_Eye.h"
#include "Pick_Ups.h"

#include <stdlib.h>
#include <math.h>

/**
 * @brief	Eye load.
 *
 * @return	null if it fails, else a Eye*.
 */
Eye *Eye_Load(int x, int y)
{
	Eye *eye;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	eye = Entity_New("images/Eyesheet.png", 100,100, gPos);

	if(eye)
	{
		eye->think = &Eye_Think;
		eye->touch = &Eye_Touch;
		eye->update = &Eye_Update;

		eye->type = ENEMY;
		eye->bounds = rect(25, 25, 50,50);
		eye->strength = 3;
		eye->speed = 2;
		eye->health = 4;
		eye->maxHealth = 4;
		vec2d_Set(eye->vel,1,1);
		eye->thinkRate = 2500;
		eye->nextThink = 0;

		eye->owner = NULL;
		eye->target = Entity_GetByID(0);
		return eye;
	}
	return NULL;
}

/**
 * @brief	Eye think.
 *
 * @param [in,out]	eye	If non-null, the eye.
 */
void Eye_Think(Eye *eye)
{	
	Vec2d vel;
	vec2d_Set(vel,5,5);
	vec2d_Subtract(eye->target->pos,eye->pos,eye->direction);
	if(SDL_GetTicks() >= eye->nextThink)
	{
		if((eye->direction.x <= 500 && eye->direction.x >= -500) && (eye->direction.y <= 300 && eye->direction.y >= -300))
		{
			Weapon_Fire(eye, vel);
			eye->nextThink = SDL_GetTicks() + eye->thinkRate;
		}
	}
}

/**
 * @brief	Eye update.
 *
 * @param [in,out]	eye	If non-null, the eye.
 */
void Eye_Update(Eye *eye)
{
	int itemPick;
	Vec2d finalPos;
	vec2d_Set(eye->vel, 1, 1);
	if (abs(eye->target->pos.y-eye->pos.y) > abs(eye->target->pos.x-eye->pos.x)) 
	{
		if ((eye->target->pos.y-eye->pos.y) > 0) 
		{
			eye->frame = 0;
		} 
		else if ((eye->target->pos.y-eye->pos.y) < 0) 
		{
			eye->frame = 2;
		}
	} 
	else
	{
		if ((eye->target->pos.x-eye->pos.x) == 0 && (eye->target->pos.y-eye->pos.y) == 0) 
		{
			eye->frame = 0;
		}
		if ((eye->target->pos.x-eye->pos.x) > 0) 
		{
			eye->frame = 3;
		} 
		else if ((eye->target->pos.x-eye->pos.x) < 0) 
		{
			eye->frame = 1;
		}
	}	
	if(rect_intersect(rect(eye->pos.x+25, eye->pos.y+25,75,75), eye->target->attack))
	{
		itemPick = rand() % 30;
		finalPos = eye->pos;
		Entity_Free(&eye);
		if(itemPick % 3 == 0)
			Pickup_Spawn(Pickup_Heart_New(finalPos));
		else if(itemPick % 5 == 0)
			Pickup_Spawn(Pickup_TempHeart_New(finalPos));
		else
			Pickup_Spawn(NULL);
	}
	Entity_IntersectAll(eye);
}

/**
 * @brief	Eye touch.
 *
 * @param [in,out]	eye	If non-null, the eye.
 */

void Eye_Touch(Eye *eye, Entity *other)
{
	Vec2d force;
	if(other == eye->target)
	{
		vec2d_Set(force,15,15);
		eye->target->health -= .5;
		vec2d_Multiply(eye->velocity9, force, eye->velocity9);
		vec2d_Add(eye->target->pos,eye->velocity9,eye->target->pos);
	}
}