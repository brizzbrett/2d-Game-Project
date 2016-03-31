#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "Camera.h"


static Sprite *spriteList = NULL;
int spriteMax = 10000;
int numSprites = 0;

void sprite_CloseSystem();
/**   
 * @brief	Initialises the Sprite System by allocating a block of memory. 
 */
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

/**
 * @brief	Frees up the memory allocated by the pointer to the sprite pointer.
 * @param	**sprite	If not null, a pointer to the sprite pointer.
 */
void sprite_Free(Sprite **sprite)
{
	Sprite *target = *sprite;
	
	if(!sprite)return;
	if(!*sprite)return;	
	target->refCount--;
	if(target->refCount == 0)
	{
		strcpy(target->filename,"\0");

		if(target->image != NULL)
			SDL_DestroyTexture(target->image);

		target->image = NULL;
	}

	*sprite = NULL;
}


/**   
 * @brief	Closes the Sprite System by freeing any memory used by the system
 */
void sprite_CloseSystem()
{
	int i;
	for(i = 0; i < numSprites; i++)
	{
		SDL_DestroyTexture(spriteList[i].image);
	}

	memset(spriteList,0,sizeof(Sprite)*numSprites);
	free(spriteList);
	spriteList = NULL;	
	numSprites = 0;
}

/**
 * @brief	Loads a new empty Sprite
 * @param	file	The file that holds the spritesheet for the sprite.
 * @param	fw  	The frame width.
 * @param	fh  	The frame height.
 * @return	the new Sprite.
 */
Sprite *sprite_Load(char *file, int fw, int fh)
{
	int i;
	SDL_Surface *temp;
	SDL_Texture *tempTex;

	if(!spriteList)return NULL;

	for(i = 0; i < numSprites; i++)
	{
		if(strncmp(file,spriteList[i].filename,128)==0)
		{
			spriteList[i].refCount++;

			return &spriteList[i];
		}
	}

	numSprites++;

	if(numSprites + 1 >= spriteMax)
	{
		fprintf(stderr, "Maximum Sprites Reached.\n");
		exit(1);
	}

	 

	for(i = 0;i <= numSprites; i++)
	{
		if(!spriteList[i].refCount)
			break;
	}

	temp = IMG_Load(file);

	if(temp == NULL)
	{
		fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
		return NULL;
	}

	SDL_SetColorKey(temp, SDL_TRUE , SDL_MapRGB(temp->format, 255,255,255));

	tempTex = SDL_CreateTextureFromSurface(Graphics_GetActiveRenderer(),temp);
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
	spriteList[i].frameSize.x = fw;
	spriteList[i].frameSize.y = fh;
	
	return &spriteList[i];	
}

/**
 * @brief	Draws a sprite to the screen
 * @param	*sprite  			If not null, the sprite being drawn.
 * @param	frame				The frame of the spritesheet being rendered.
 * @param	*renderer			If not null, the renderer being drawn to.
 * @param	pos					The position on the screen the sprite will show up.
 */
void sprite_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos)
{
	SDL_Rect src,dest;
	Vec2d posRel;
	Vec2d camPos;
	SDL_Rect cam;
	cam = Camera_GetActiveCamera();
	camPos = Camera_GetPosition();
	vec2d_Subtract(pos, camPos, posRel); 

	src.x = frame%sprite->fpl * sprite->frameSize.x;
	src.y = frame/sprite->fpl * sprite->frameSize.y;
	src.w = sprite->frameSize.x;
	src.h = sprite->frameSize.y;

	dest.x = posRel.x;
	dest.y = posRel.y;
	dest.w = sprite->frameSize.x;
	dest.h = sprite->frameSize.y;

	SDL_RenderCopy(renderer,sprite->image, &src, &dest);
}