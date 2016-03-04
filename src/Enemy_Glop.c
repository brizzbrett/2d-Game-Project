#include "Enemy_Glop.h"
#include "Entity.h"
#include "Player.h"
#include "Vector.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <math.h>

static Vec2d pPos;
static Vec2d pSize;
static Vec2d gPos;
static Vec2d gSize;
static Vec2d direction;
static Vec2d velocity;

/**
 * @brief	Glop load.
 *
 * @return	null if it fails, else a Glop*.
 */
Glop *Glop_Load()
{
	Glop *glop;

	Vec2d pos;
	vec2d_Set(pos,800,800);

	glop = Entity_New("images/glopsheet.png", 100,100, pos);

	if(glop)
	{
		glop->think = &Glop_Think;
		glop->touch = &Glop_Touch;
		glop->update = &Glop_Update;

		glop->id = 1;
		glop->type = ENEMY;
		glop->bounds = rect(glop->pos.x, glop->pos.y, glop->sprite->frameSize.x,glop->sprite->frameSize.y);
		glop->strength = 3;
		glop->speed = 2;
		glop->health = 4;
		glop->maxHealth = 4;
		vec2d_Set(glop->vel,1,1);

		glop->owner = NULL;
		glop->target = Entity_GetByID(0);
		gPos = glop->pos;

		vec2d_Set(gSize,glop->sprite->frameSize.x/2,glop->sprite->frameSize.y/2);
		vec2d_Set(pSize,glop->target->sprite->frameSize.x/2,glop->target->sprite->frameSize.y/2);
		return glop;
	}
	return NULL;
}

/**
 * @brief	Glop think.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */
void Glop_Think(Glop *glop)
{	


	vec2d_Add(glop->target->pos,pSize,pPos); 
	vec2d_Subtract(pPos,gPos,direction);
	vec2d_Normalize(&direction);
	vec2d_Multiply(glop->vel,direction,velocity);
	vec2d_Subtract(gPos,gSize,glop->pos);
	vec2d_Add(gPos,velocity,gPos);
	

	if (abs(glop->target->pos.y-glop->pos.y) > abs(glop->target->pos.x-glop->pos.x)) 
	{
		if ((glop->target->pos.y-glop->pos.y) > 0) 
		{
			glop->frame = 0;
		} 
		else if ((glop->target->pos.y-glop->pos.y) < 0) 
		{
			glop->frame = 2;
		}
	} 
	else
	{
		if ((glop->target->pos.x-glop->pos.x) == 0 && (glop->target->pos.y-glop->pos.y) == 0) 
		{
			glop->frame = 0;
		}
		if ((glop->target->pos.x-glop->pos.x) > 0) 
		{
			glop->frame = 3;
		} 
		else if ((glop->target->pos.x-glop->pos.x) < 0) 
		{
			glop->frame = 1;
		}
	}
}

/**
 * @brief	Glop update.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */
void Glop_Update(Glop *glop)
{
	vec2d_Set(glop->vel, 1, 1);
}

/**
 * @brief	Glop touch.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */

void Glop_Touch(Glop *glop)
{
	Vec2d force;
	vec2d_Set(force,50,50);
	glop->target->health -= .5;
	slog("%i", glop->target->health);
	vec2d_Multiply(velocity, force, velocity);
	vec2d_Add(glop->target->pos,velocity,glop->target->pos);
}