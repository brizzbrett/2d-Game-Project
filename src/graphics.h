#ifndef _GRAPHICS_
#define _GRAPHICS_
/*
 *			graphics.h
 *			Originaly written by Donald Kehoe for the NJIT Introduction to game development class
 *			This file contains data structures, constants and function prototypes needed to initialize SDL and
 *			to handle graphics.
 */

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)

/**
 * @brief	Graphics initialise.
 * @param [in,out]	windowName	If non-null, name of the window.
 * @param	viewW			  	The view width.
 * @param	viewH			  	The view heigth.
 * @param	renderW			  	The render width.
 * @param	renderH			  	The render height.
 * @param	bgcolor			  	The background color.
 * @param	fullscreen		  	The fullscreen.
 */
void Graphics_Init(char *windowName,int renderW,int renderH,int fullscreen);

/**
 * @brief gets a pointer to the current active renderer;
 * @return return a pointer to the active rendering context;
 */
SDL_Renderer *Graphics_GetActiveRenderer();

/**
 * @brief	Graphics get system time.
 * @return	An Uint32.
 */
Uint32 Graphics_GetSystemTime();

/**
 * @brief	Frame delay.
 * @param	delay	The delay.
 */
void FrameDelay(Uint32 delay);

/**   
 * @brief	Resets the buffer. 
 */
void ResetBuffer();

/**   
 * @brief	Next frame. 
 */
void NextFrame();

#endif