#include "Player.h"
#include "Camera.h"
#include <math.h>

Entity *player;
Entity *Player_Load(int x, int y)
{

	Vec2d pos;
	vec2d_Set(pos,x,y);
	player = Entity_New(PLAYER, pos);
	if(player)
	{
		player->think = &Player_Think; 
		player->touch = &Player_Touch;
		player->update = &Player_Update;

		player->attack = rect(0,0,0,0);

		player->owner = NULL;
		player->target = NULL;
		player->sound = Sound_New("audio/punch.ogg",0,PLAYER_FX);

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
			Sound_Player(player->sound);
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
	if(!Camera_Intersect(player))
	{
		player->flag = 1;	
	}
	vec2d_Add(player->pos, player->vel, player->pos);
	//slog("%f", player->health);
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