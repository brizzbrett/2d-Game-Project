#include "sprite.h"
#include <stdlib.h>

Sprite *spriteList = NULL;
int spriteMax = 1000;

void SpriteInitSystem()
{
	spriteList = (Sprite *)malloc(sizeof(Sprite)*spriteMax);
	memset(spriteList,0,sizeof(Sprite)*spriteMax);
	atexit(sprite_CloseSystem);
}
void sprite_CloseSystem();

Sprite *loadSprite(char file[], int fw, int fh)
{

}
void freeSprite(Sprite *sprite);
void drawSprite(Sprite *sprite, int frame, SDL_Renderer *renderer, int drawX, int drawY);