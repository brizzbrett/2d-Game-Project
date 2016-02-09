#ifndef __SPRITE_H__
#define __SPRITE_H__


#include <SDL.h>

typedef struct Sprite_T
{
	int refCount;
	char filename[128];
	SDL_TEXTURE image;
	int imageW, imageH;
	int frameW. frameH;
	int fpl;

}Sprite;

void sprite_InitSystem();
void sprite_CloseSystem();

Sprite *sprite_Load(char file[], int fw, int fh);
void sprite_Free(Sprite *sprite);
void sprite_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, int drawX, int drawY);

#endif