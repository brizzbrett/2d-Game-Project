#include "Enemy_Glop.h"
#include "Pick_Ups.h"

#include <stdlib.h>
#include <math.h>
#include "Camera.h"

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

	glop = Entity_New("images/glopsheet.png",128,128, gPos);

	if(glop)
	{
		glop->think = NULL;
		glop->touch = &Glop_Touch;
		glop->update = &Glop_Update;

		glop->type = ENEMY;
		glop->bounds = rect(40, 40, 60,113);
		glop->strength = 3;
		glop->speed = 2;
		glop->health = 4;
		glop->maxHealth = 4;
		vec2d_Set(glop->vel,1,1);

		glop->owner = NULL;
		glop->target = Entity_GetByType(PLAYER);
		return glop;
	}
	return NULL;
}

void Glop_Think(Glop *glop)
{		
	vec2d_Subtract(glop->target->pos,glop->pos,glop->direction);
	vec2d_Normalize(&glop->direction);
	vec2d_Multiply(glop->vel,glop->direction,glop->velocity9);
	vec2d_Add(glop->pos,glop->velocity9,glop->pos);
	
	if (abs(glop->target->pos.y-glop->pos.y) > abs(glop->target->pos.x-glop->pos.x)) 
	{
		if ((glop->target->pos.y-glop->pos.y) > 0) 
		{
			glop->frame = 0;
			glop->bounds = rect(50, 50, 40,83);
		} 
		else if ((glop->target->pos.y-glop->pos.y) < 0) 
		{
			glop->frame = 2;
			glop->bounds = rect(50, 50, 60,83);
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
			glop->bounds = rect(40, 40, 60,113);
		} 
		else if ((glop->target->pos.x-glop->pos.x) < 0) 
		{
			glop->frame = 1;
			glop->bounds = rect(40, 40, 60,113);
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
	int itemPick;
	Vec2d finalPos;

	if(!glop)return;
	vec2d_Set(glop->vel, 0.02, 0.02);
	glop->target = Entity_GetByType(PLAYER);

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
	if(Camera_Intersect(Camera_GetActiveCamera(), glop))
	{
		glop->think = &Glop_Think;
	}
	else
	{
		if(!glop) return;
		glop->think = NULL;
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
		vec2d_Set(glop->force,2500,2500);
		glop->target->health -= .5;
		vec2d_Multiply(glop->velocity9, glop->force, glop->velocity9);
		vec2d_Add(glop->target->pos,glop->velocity9,glop->target->pos);
	}
	if(other->type == BOULDER)
	{
		
	}
}