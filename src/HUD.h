#ifndef _HUD_H_
#define _HUD_H_
#include "Player.h"
#include "Camera.h"
#include "SDL_ttf.h"

typedef struct
{
	Entity *player;
	Entity *boss;

	Sprite *playerHearts;
	Sprite *bossHearts;

	Sprite *playerTempHearts;

	Vec2d initHeartPos;
}HUD;

HUD *HUD_Init();

void HUD_Draw(HUD *HUD);


#endif