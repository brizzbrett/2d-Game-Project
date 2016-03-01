#include "Player.h"
#include "Entity.h"
#include "Vector.h"
#include "simple_logger.h"
#include "Graphics.h"
#include <math.h>

extern SDL_Surface *buffer; /**<pointer to the draw buffer*/

Player *player;

Player *Player_Load()
{
	Vec2d pos;
	vec2d_Set(pos,775,600);
	player = Entity_New("images/playersheet.png", 64,64, pos);
	if(player)
	{
		player->think = &Player_Think;
		player->touch = &Player_Touch;
		player->update = &Player_Update;

		player->pos = pos;
		player->type = PLAYER;
		player->bounds = rect(player->pos.x-10, player->pos.y-10,player->sprite->frameSize.x-10,player->sprite->frameSize.y-10);
		player->strength = 3;
		player->speed = 2;
		player->health = 4;
		player->maxHealth = 4;

		return player;
	}
	return NULL;
}
Player *Player_Get()
{
	return player;
}
void Player_Think(Entity *ent)
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
void Player_Update(Entity *ent)
{
	slog("%i",player->health);
}
void Player_Touch(Entity *ent)
{
	vec2d_Set(player->vel, 0, 0);
}
int You_Died()
{
	if(player->health <= 0)
		return true;
	return false;
}