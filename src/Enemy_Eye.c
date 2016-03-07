#include "Enemy_Eye.h"
#include "Enemy_Fire.h"
#include "Entity.h"
#include "Player.h"
#include "Vector.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <math.h>

static Vec2d direction;
static Vec2d velocity;

/**
 * @brief	Eye load.
 *
 * @return	null if it fails, else a Eye*.
 */
Eye *Eye_Load(int id, int x, int y)
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

		eye->id = id;
		eye->type = ENEMY;
		eye->bounds = rect(eye->pos.x, eye->pos.y, eye->sprite->frameSize.x,eye->sprite->frameSize.y);
		eye->strength = 3;
		eye->speed = 2;
		eye->health = 4;
		eye->maxHealth = 4;
		vec2d_Set(eye->vel,1,1);
		eye->thinkRate = 1500;
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
	if(SDL_GetTicks() >= eye->nextThink)
	{

		Weapon_Fire(eye);
		eye->nextThink = SDL_GetTicks() + eye->thinkRate;
	}
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
}

/**
 * @brief	Eye update.
 *
 * @param [in,out]	eye	If non-null, the eye.
 */
void Eye_Update(Eye *eye)
{
	vec2d_Set(eye->vel, 1, 1);
}

/**
 * @brief	Eye touch.
 *
 * @param [in,out]	eye	If non-null, the eye.
 */

void Eye_Touch(Eye *eye)
{
	Vec2d force;
	vec2d_Set(force,50,50);
	eye->target->health -= .5;
	vec2d_Multiply(velocity, force, velocity);
	vec2d_Add(eye->target->pos,velocity,eye->target->pos);
}