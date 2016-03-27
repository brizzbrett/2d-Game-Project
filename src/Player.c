#include "Player.h"
#include <math.h>

Entity *player;
Entity *Player_Load(int x, int y)
{

	Vec2d pos;
	vec2d_Set(pos,x,y);
	player = Entity_New("images/playersheet.png", 128,128, pos);
	if(player)
	{
		player->think = &Player_Think;
		player->touch = &Player_Touch;
		player->update = &Player_Update;

		player->id = 0;
		player->pos = pos;
		player->type = PLAYER;
		player->bounds = rect(38, 5,player->sprite->frameSize.x-76,player->sprite->frameSize.y-10);
		player->attack = rect(0,0,0,0);
		player->strength = 3;
		player->speed = 5;
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
void Player_Think(Entity *player)
{
	
	SDL_Event e;
	if(!player)return;

	if( SDL_PollEvent( &e ) != 0 )
	{
		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			Player_Attack();
		}
	}
	if(SDL_GetTicks() >= player->nextThink)
	{
		player->attack = rect(0,0,0,0); 
		player->nextThink = SDL_GetTicks() + player->thinkRate;
	}
}
void Player_Update(Entity *player)
{
	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_W])
	{
		player->direction.y = -1;
		player->vel.y = player->direction.y * player->speed;
		
		player->frame = 2;
	}
	else if(keys[SDL_SCANCODE_S])
	{
		player->direction.y = 1;
		player->vel.y = player->direction.y * player->speed;
		player->frame = 1;
	}
	else
	{
		player->vel.y = 0;
	}
	if(keys[SDL_SCANCODE_A])
	{
		player->direction.x = -1;
		player->vel.x = player->direction.x * player->speed;
		player->frame = 3;
	}
	else if(keys[SDL_SCANCODE_D])
	{
		player->direction.x = 1;
		player->vel.x = player->direction.x * player->speed;
		player->frame = 0;
	}
	else
	{
		player->vel.x = 0;
	}
	vec2d_Add(player->pos, player->vel, player->pos);
	slog("%f", player->health);
	Entity_IntersectAll(player);
}
void Player_Touch(Entity *player, Entity *other)
{
	Uint32 timer;
	timer = SDL_GetTicks()+100;
	if(SDL_GetTicks() >= timer)
	{
		vec2d_Set(player->vel, 3,3);
	}
}
void Player_Attack()
{
	SDL_Rect attack;
	Vec2d offset;
	
	if(player)
	{
		if(player->frame == 1)
		{
			vec2d_Set(offset,30,player->sprite->frameSize.y);
		}
		else if(player->frame == 2)
		{
			vec2d_Set(offset,30,-40);
		}
		else if(player->frame == 0)
		{
			vec2d_Set(offset,player->sprite->frameSize.x,50);
		}
		else if(player->frame == 3)
		{
			vec2d_Set(offset,-40, 50);
		}

		attack = rect(player->pos.x + offset.x,player->pos.y + offset.y, 40,40);
		player->attack = attack;	
	}
}