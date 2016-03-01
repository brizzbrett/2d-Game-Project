#include "Enemy_Glop.h"
#include "Entity.h"
#include "Player.h"
#include "Vector.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <math.h>

Glop *glop;
Vec2d pPos;
Vec2d pSize;
Vec2d gPos;
Vec2d gSize;
Vec2d direction;
Vec2d velocity;

Glop *Glop_Load()
{
	Vec2d pos;
	vec2d_Set(pos,300,250);

	glop = (Glop *)malloc(sizeof(Glop));

	glop = Entity_New("images/glopsheet.png", 100,100, pos);
	gPos = glop->pos;

	vec2d_Set(gSize,glop->sprite->frameSize.x/2,glop->sprite->frameSize.y/2);
	vec2d_Set(pSize,Player_Get()->sprite->frameSize.x/2,Player_Get()->sprite->frameSize.y/2);
	if(glop)
	{
		glop->think = &Glop_Think;
		glop->touch = &Glop_Touch;
		glop->update = &Glop_Update;

		glop->type = ENEMY;
		glop->bounds = rect(glop->pos.x, glop->pos.y, glop->sprite->frameSize.x,glop->sprite->frameSize.y);
		glop->strength = 3;
		glop->speed = 2;
		glop->health = 4;
		glop->maxHealth = 4;
		vec2d_Set(glop->vel,1,1);

		return glop;
	}
	return NULL;
}
void Glop_Think(Entity *player)
{
	player = Player_Get();
	vec2d_Add(player->pos,pSize,pPos); 
	vec2d_Subtract(pPos,gPos,direction);
	vec2d_Normalize(&direction);
	vec2d_Multiply(glop->vel,direction,velocity);
	vec2d_Subtract(gPos,gSize,glop->pos);
	vec2d_Add(gPos,velocity,gPos);
	

	if (abs(player->pos.y-glop->pos.y) > abs(player->pos.x-glop->pos.x)) 
	{
		if ((player->pos.y-glop->pos.y) > 0) 
		{
			glop->frame = 0;
		} 
		else if ((player->pos.y-glop->pos.y) < 0) 
		{
			glop->frame = 2;
		}
	} 
	else
	{
		if ((player->pos.x-glop->pos.x) == 0 && (Player_Get()->pos.y-glop->pos.y) == 0) 
		{
			glop->frame = 0;
		}
		if ((player->pos.x-glop->pos.x) > 0) 
		{
			glop->frame = 3;
		} 
		else if ((player->pos.x-glop->pos.x) < 0) 
		{
			glop->frame = 1;
		}
	}
}
void Glop_Update(Entity *ent)
{
	vec2d_Set(glop->vel, 1, 1);
}
void Glop_Touch(Player *player)
{
	Vec2d force;
	vec2d_Set(force,50,50);
	Player_Get()->health -= .5;
	slog("%i", player->health);
	vec2d_Multiply(velocity, force, velocity);
	vec2d_Add(Player_Get()->pos,velocity,Player_Get()->pos);
}