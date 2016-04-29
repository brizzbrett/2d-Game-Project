#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Camera.h"
#include "SDL_ttf.h"
#include "HUD.h"
#include "SDL_image.h"
#include "chipmunk.h"

HUD *HUD_Init()
{
	HUD *temp;
	Vec2d initHeartPos;
	
	vec2d_Set(initHeartPos, 100, 50);
	
	temp = (HUD *)malloc(sizeof(HUD));
	temp->player = Entity_GetByType(PLAYER);
	temp->playerHearts = sprite_Load("images/heart2.png",100,100);
	temp->playerTempHearts = sprite_Load("images/tempheart2.png",100,100);
	temp->initHeartPos = initHeartPos;

	return temp;
}

void HUD_Draw(HUD *hud)
{
	int i;
	int k;
	int m;
	Vec2d j;
	Vec2d temp;

	vec2d_Set(j, Camera_GetPosition().x+25, Camera_GetPosition().y+50);

	for(i = 0; i < hud->player->health; i++)
	{
		
		if(i < hud->player->maxHealth)
		{
			j.x = j.x+40;
			sprite_Draw(hud->playerHearts,0,Graphics_GetActiveRenderer(),j);
		}

	}		
	for(m = hud->player->maxHealth; m < hud->player->health; m++)
	{
		j.x = j.x+40;
		sprite_Draw(hud->playerTempHearts,0,Graphics_GetActiveRenderer(),j);	
	}
}	