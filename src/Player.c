#include "Player.h"
#include "Entity.h"
#include "Vector.h"
#include <math.h>

Player *player;

Player *Player_Load()
{
	Vec2d pos;
	vec2d_Set(pos,100,100);
	player = Entity_New("images/playersheet.png", 27,48, pos);
	player->draw = &sprite_Draw;
	player->think = &Player_Think;

	player->strength = 3;
	player->speed = 2;
	player->health = 4;
	player->maxHealth = 4;

	return player;
}
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
}
void Player_Update();
void Player_Attack();