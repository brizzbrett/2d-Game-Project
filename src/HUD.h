#ifndef _HUD_H_
#define _HUD_H_
#include "Player.h"
#include "Camera.h"
#include "SDL_ttf.h"

/**
*@brief define the Gui object for HUB purposes
*@param TTF_Font* font the font to be used ofr the HUD
*@param SDL_Surface *timer the surface to store the game time
*@param SDL_Surface *speed the surface to store the player speed
*@param SDL_Rect speeddst the rectangle to output the player speed
*@param SDL_Rect timedst the rectangle to output the game time
*@param SDL_Texture* HUD1 The texture used to output the player speed
*@param SDL_Texture* HUD2 The texture used to output the game time
*@param SDL_Color textColor The color of the font
*/
typedef struct
{

	TTF_Font* font; 
	SDL_Surface *timer;
	SDL_Surface *speed;
	SDL_Surface *rank;
	SDL_Surface *mph;
	SDL_Surface *ranker;

	SDL_Rect speeddst;
	SDL_Rect timedst;
	SDL_Rect rankdst;
	SDL_Texture* hud1; //speed digit 1
	SDL_Texture* hud2; // speed digit 10
	SDL_Texture* hud3;
	SDL_Color textColor;
	SDL_Color textColor2;

	SDL_Rect speeddst2; //the mph word
	SDL_Rect rankdst2; //the rank word
	SDL_Texture* hud4;
	SDL_Texture* hud5;

	Entity *player;
	Sprite *playerHearts;
	Sprite *playerTempHearts;
	Vec2d initHeartPos;

}HUD;
/**
*@brief draws the time and speed of the car
*@return a HUD type object to be used for DrawGui
*/
HUD *InitHUD();

/**
*@brief draws the time and speed of the car
*@param HUD the specific HUD object used for rendering
*@param *player used to output player speed
*@param font display font
*/
void DrawGui(HUD *HUD);


#endif