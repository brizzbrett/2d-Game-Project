#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Sprite *spriteList = NULL;
int spriteMax = 1000;
int numSprites = 0;

void sprite_InitSystem()
{
	int i;
	if(spriteMax == 0)
	{
		printf("Max Sprite == 0\n");
		return;
	}
	spriteList = (Sprite *)malloc(sizeof(Sprite)*spriteMax);
	memset(spriteList,0,sizeof(Sprite)*spriteMax);
	for(i = 0; i < spriteMax; i++)
	{
		spriteList[i].image = NULL;
	}
	numSprites = 0;
	atexit(sprite_CloseSystem);
}

void sprite_Free(Sprite *sprite)
{
	sprite->refCount--;
	if(sprite->refCount == 0)
	{
		strcpy(sprite->filename,"\0");
		if(sprite->image != NULL)
			SDL_DestroyTexture(sprite->image);
		sprite->image = NULL;
	}
}

void sprite_CloseSystem()
{
	int i;
	for(i = 0; i < spriteMax; i++)
	{
		sprite_Free(&spriteList[i]);
	}
	numSprites = 0;
	memset(spriteList,0,sizeof(Sprite)*spriteMax);
}

Sprite *sprite_Load(SDL_Renderer *renderer, char file[], int fw, int fh)
{
	int i;
	SDL_Surface *temp;
	SDL_Texture *tempTex;
	for(i = 0; i < numSprites; ++i)
	{
		if(strncmp(file,spriteList[i].filename,128)==0)
		{
			spriteList[i].refCount++;
			return &spriteList[i];
		}
	}
	if(numSprites + 1 >= spriteMax)
	{
		fprintf(stderr, "Maximum Sprites Reached.\n");
		exit(1);
	}

	numSprites++;

	/*for(i = 0;i <= numSprites; i++)
	{
		if(!spriteList[i].refCount)
			break;
	}*/

	temp = IMG_Load(file);

	if(temp == NULL)
	{
		fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
		exit(0);
	}

	SDL_SetColorKey(temp, SDL_TRUE , SDL_MapRGB(temp->format, 255,255,255));

	tempTex = SDL_CreateTextureFromSurface(renderer,temp);
	if(tempTex == NULL)
	{
		fprintf(stderr,"unable to load a vital sprite as texture: %s\n",SDL_GetError());
		exit(0);
	}

	SDL_FreeSurface(temp);

	spriteList[i].image = tempTex;
	spriteList[i].refCount++;
	strncpy(spriteList[i].filename,file,128);
	spriteList[i].fpl = 16;
	spriteList[i].frameW = fw;
	spriteList[i].frameH = fh;
	

	return &spriteList[i];	
}

void sprite_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, int drawX, int drawY)
{
	SDL_Rect src,dest;
	src.x = frame%sprite->fpl * sprite->frameW;
	src.y = frame/sprite->fpl * sprite->frameH;
	src.w = sprite->frameW;
	src.h = sprite->frameH;

	dest.x = drawX;
	dest.y = drawY;
	dest.w = sprite->frameW;
	dest.h = sprite->frameH;
	SDL_RenderCopy(renderer,sprite->image, &src, &dest);
}