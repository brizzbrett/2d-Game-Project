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
	temp->boss = Entity_GetByType(NIGHTBOSS);
	temp->playerHearts = sprite_Load("images/heart2.png",100,100);
	temp->playerTempHearts = sprite_Load("images/tempheart2.png",100,100);
	temp->initHeartPos = initHeartPos;

	return temp;
}

void HUD_Draw(HUD *hud)
{
	int i;
	int m;
	Vec2d playerHeart;
	Vec2d bossHeart;

	vec2d_Set(playerHeart, Camera_GetPosition().x+25, Camera_GetPosition().y+50);
	vec2d_Set(bossHeart, Camera_GetPosition().x+10, Camera_GetPosition().y+620);

	for(i = 0; i < hud->player->health; i++)
	{
		
		if(i < hud->player->maxHealth)
		{
			playerHeart.x = playerHeart.x+40;
			sprite_Draw(hud->playerHearts,0,Graphics_GetActiveRenderer(),playerHeart,1);
		}

	}		
	for(m = hud->player->maxHealth; m < hud->player->health; m++)
	{
		playerHeart.x = playerHeart.x+40;
		sprite_Draw(hud->playerTempHearts,0,Graphics_GetActiveRenderer(),playerHeart,1);	
	}

	hud->boss = Entity_GetByType(NIGHTBOSS);
	if(hud->boss && hud->boss->think != NULL)
	{
		for(i = 0; i < hud->boss->health; i++)
		{
		
			if(i < hud->boss->maxHealth)
			{
				bossHeart.x = bossHeart.x+40;
				sprite_Draw(hud->playerHearts,0,Graphics_GetActiveRenderer(),bossHeart,1);
			}

		}		
	}
}	