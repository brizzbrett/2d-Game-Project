#include "Player.h"
#include "Entity.h"
#include "Vector.h"
#include "simple_logger.h"
#include "Graphics.h"
#include <math.h>

Player *player;
Player *Player_Load(int x, int y)
{

	Vec2d pos;
	vec2d_Set(pos,x,y);
	player = Entity_New("images/playersheet.png", 64,64, pos);
	if(player)
	{
		player->think = &Player_Think;
		player->touch = &Player_Touch;
		player->update = &Player_Update;

		player->id = 0;
		player->pos = pos;
		player->type = PLAYER;
		player->bounds = rect(player->pos.x, player->pos.y,player->sprite->frameSize.x,player->sprite->frameSize.y);
		player->attack = rect(0,0,0,0);
		player->strength = 3;
		player->speed = 3;
		player->health = 4;
		player->maxHealth = 4;

		player->thinkRate = 100;
		player->nextThink = 0;

		player->owner = NULL;
		player->target = NULL;

		return player;
	}
	return NULL;
}
void Player_Attack();
void Player_Think(Player *player)
{
	const Uint8 *keys;

	if(!player)return;

	keys = SDL_GetKeyboardState(NULL);
	if(keys[SDL_SCANCODE_W])
	{
		player->vel.y = -player->speed;
		player->frame = 1;
	}
	else if(keys[SDL_SCANCODE_S])
	{
		player->vel.y = player->speed;
		player->frame = 0;
	}
	else
	{
		player->vel.y = 0;
	}
	if(keys[SDL_SCANCODE_A])
	{
		player->vel.x = -player->speed;
		player->frame = 3;
	}
	else if(keys[SDL_SCANCODE_D])
	{
		player->vel.x = player->speed;
		player->frame = 2;
	}
	else
	{
		player->vel.x = 0;
	}

	if(keys[SDL_SCANCODE_Z])
	{
		Player_Attack();
	}

}
void Player_Update(Player *player)
{
	vec2d_Add(player->pos, player->vel, player->pos);
}
void Player_Touch(Player *player)
{
	vec2d_Set(player->vel, 0, 0);
}
void Player_Attack()
{
	SDL_Rect attack;
	Vec2d offset;
	Vec2d size;
	
	if(player)
	{
		if(player->frame == 0)
		{
			vec2d_Set(offset,40,50);
			vec2d_Set(size,20,70);
		}
		else if(player->frame == 1)
		{
			vec2d_Set(offset,40,-20);
			vec2d_Set(size,20,70);
		}
		else if(player->frame == 2)
		{
			vec2d_Set(offset,-20,50);
			vec2d_Set(size,70,20);
		}
		else if(player->frame == 3)
		{
			vec2d_Set(offset,60, 50);
			vec2d_Set(size,70,20);
		}
		attack = rect(player->pos.x + offset.x,player->pos.y + offset.y, size.x,size.y);
		player->attack = attack;
	}
}