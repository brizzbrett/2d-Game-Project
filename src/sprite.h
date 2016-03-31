#ifndef __SPRITE_H__
#define __SPRITE_H__


#include <SDL.h>

#include "Vector.h"

/** @brief	Defines an alias representing the sprite t. */
typedef struct Sprite_T
{
	int refCount;
	char filename[128];
	SDL_Texture *image;
	Vec2d imageSize;
	Vec2d frameSize;
	int fpl;

}Sprite;

/**   
 * @brief	Initialises the Sprite System. 
 */
void sprite_InitSystem();

/**   
 * @brief	Closes the Sprite System 
 */
void sprite_CloseSystem();

/**
 * @brief	Loads a new empty Sprite
 * @param	file	The file that holds the spritesheet for the sprite.
 * @param	fw  	The frame width.
 * @param	fh  	The frame height.
 * @return	the new Sprite.
 */
Sprite *sprite_Load(char *file, int fw, int fh);

/**
 * @brief	Frees up the memory allocated by the pointer to the sprite pointer.
 * @param	**sprite	If not null, a pointer to the sprite pointer.
 */
void sprite_Free(Sprite **sprite);

/**
 * @brief	Sprite draw.
 * @param	*sprite  			If not null, the sprite being drawn.
 * @param	frame				The frame of the spritesheet being rendered.
 * @param	*renderer			If not null, the renderer being drawn to.
 * @param	pos					The position on the screen the sprite will show up.
 */
void sprite_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);

#endif