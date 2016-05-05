#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "Camera.h"
#include <math.h>


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
		if(spriteList[i].filename == file)
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
	spriteList[i].filename = file;
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

	if(!sprite)return;

	cam = Camera_GetActiveCamera();
	camPos = Camera_GetPosition();
	vec2d_Subtract(pos, camPos,  posRel); 

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

void sprite_BloomDraw(Sprite *bloom, int frame, SDL_Renderer *renderer, Vec2d pos, Vec3d rgb, int sz)
{	
	SDL_Rect src, dest;
	Vec2d posRel;
	Vec2d camPos;
	int i;
	int size;
	float offsetX, offsetY;
	int bloomCount, range, min;
	int variance = sz / 10+1;

	SDL_Rect cam;

	cam = Camera_GetActiveCamera();
	camPos = Camera_GetPosition();
	vec2d_Subtract(pos, camPos, posRel); 

	src.x = frame%bloom->fpl*bloom->frameSize.x;
	src.y = frame/bloom->fpl*bloom->frameSize.y;
	src.w = bloom->frameSize.x;
	src.h = bloom->frameSize.y;
	
	bloomCount = 10 * sin(2 * 3.14 * SDL_GetTicks()/1000) + 15; 
	SDL_SetTextureAlphaMod(bloom->image, 15);
	//SDL_SetTextureBlendMode(bloom->image, SDL_BLENDMODE_ADD);
	for(i = 0; i < bloomCount;i++)
	{
		size = rand() % sz;	
		offsetX = sin(2 * 3.14 * SDL_GetTicks()/1000) * (variance*2) - variance;
		offsetY = sin(2 * 3.14 * SDL_GetTicks()/1000) * (variance*2) - variance;
		dest.x = posRel.x - size / 2 + offsetX;
		dest.y = posRel.y - size / 2 + offsetY;
		dest.w = bloom->frameSize.x + size;
		dest.h = bloom->frameSize.y + size;
		
		SDL_SetTextureColorMod(bloom->image, rgb.r - (offsetX + offsetY) * 10, rgb.g - (offsetX + offsetY) * 10, rgb.b);
			
		SDL_RenderCopy(renderer, bloom->image, &src, &dest);	
	}
	SDL_SetTextureBlendMode(bloom->image, SDL_BLENDMODE_BLEND);

	SDL_SetTextureAlphaMod(bloom->image, 255);
	SDL_SetTextureColorMod(bloom->image, rgb.r, rgb.g, rgb.b);
}


Sprite* sprite_LoadText(TTF_Font *font, char *text, SDL_Color color)
{
	int i;
	Sprite *sprite = NULL;
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;
	SDL_Renderer *renderer;

	if(!font)return NULL;
	if(!text)return NULL;

	if(!spriteList)return NULL;

	renderer = Graphics_GetActiveRenderer();

	for(i = 0; i < spriteMax; i++)
	{
		if(spriteList[i].refCount == 0)
		{
			if(sprite == NULL)
			{
				sprite = &spriteList[i];
			}
			continue;
		}
		if(spriteList[i].filename == text)
		{
			spriteList[i].refCount++;
			return &spriteList[i];
		}
	}
	if(numSprites + 1 > spriteMax)
	{
		slog("Maximum Sprites Reached.");
		exit(1);
	}

	textSurface = TTF_RenderText_Blended(font, text, color);
	if(!textSurface)
	{
		return NULL;
	}
	else
	{
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	}
	if(!textTexture)
	{
		return NULL;
	}

	sprite->image = textTexture;
	sprite->filename = text;
	sprite->frameSize.x = textSurface->w;
	sprite->frameSize.y = textSurface->h;
	sprite->refCount++;
	sprite->fpl = 16;
	SDL_FreeSurface(textSurface);
	return sprite;
}

void sprite_TextDraw(Sprite *text, Vec2d drawPos, int alpha)
{
	SDL_Rect src, dest;
	Vec2d posRel;
	Vec2d camPos;
	SDL_Renderer *renderer;
	SDL_Rect cam;
	
	if(!text)return;

	renderer = Graphics_GetActiveRenderer();
	cam = Camera_GetActiveCamera();
	camPos = Camera_GetPosition();
	vec2d_Subtract(drawPos, camPos, posRel); 

	src.x = 0;
	src.y = 0;
	src.w = text->frameSize.x;
	src.h = text->frameSize.y;
	
	dest.x = posRel.x;
	dest.y = posRel.y;
	dest.w = text->frameSize.x;
	dest.h = text->frameSize.y;

	SDL_SetTextureAlphaMod(text->image, alpha);
	SDL_RenderCopy(renderer, text->image, &src, &dest);
	
}