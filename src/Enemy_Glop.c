#include "Enemy_Glop.h"
#include "Pick_Ups.h"

#include <stdlib.h>
#include <math.h>

/**
 * @brief	Glop load.
 *
 * @return	null if it fails, else a Glop*.
 */
Glop *Glop_Load(int x, int y)
{
	Glop *glop;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	glop = Entity_New("images/glopsheet.png", 100,100, gPos);

	if(glop)
	{
		glop->touch = &Glop_Touch;
		glop->update = &Glop_Update;

		glop->type = ENEMY;
		glop->bounds = rect(10, 10, glop->sprite->frameSize.x-20,glop->sprite->frameSize.y-20);
		glop->strength = 3;
		glop->speed = 2;
		glop->health = 4;
		glop->maxHealth = 4;
		vec2d_Set(glop->vel,1,1);

		glop->owner = NULL;
		glop->target = Entity_GetByID(0);
		return glop;
	}
	return NULL;
}

/**
 * @brief	Glop update.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */
void Glop_Update(Glop *glop)
{
	int itemPick;
	Vec2d finalPos;
	vec2d_Set(glop->vel, 1, 1);
		
	vec2d_Subtract(glop->target->pos,glop->pos,glop->direction);
	vec2d_Normalize(&glop->direction);
	vec2d_Multiply(glop->vel,glop->direction,glop->velocity9);
	vec2d_Add(glop->pos,glop->velocity9,glop->pos);
	
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
	if(rect_intersect(rect(glop->pos.x, glop->pos.y,100,100), glop->target->attack))
	{
		itemPick = rand() % 30;
		finalPos = glop->pos;
		Entity_Free(&glop);
		if(itemPick % 3 == 0)
			Pickup_Spawn(Pickup_Heart_New(finalPos));
		else if(itemPick % 5 == 0)
			Pickup_Spawn(Pickup_TempHeart_New(finalPos));
		else
			Pickup_Spawn(NULL);	
	}
	Entity_IntersectAll(glop);
}

/**
 * @brief	Glop touch.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */

void Glop_Touch(Glop *glop, Entity *other)
{
	if(other == glop->target)
	{
		vec2d_Set(glop->force,15,15);
		glop->target->health -= .5;
		vec2d_Multiply(glop->velocity9, glop->force, glop->velocity9);
		vec2d_Add(glop->target->pos,glop->velocity9,glop->target->pos);
	}
}