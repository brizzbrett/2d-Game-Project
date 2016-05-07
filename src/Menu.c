#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "sprite.h"
#include "simple_logger.h"
#include "Menu.h"
#include "Level.h"

TTF_Font *font = NULL;
TTF_Font *font2 = NULL;

SDL_Color textColor = { 33, 3, 0 };

Sprite *bg;
Vec2d bgPos;

void Menu_TitleScreenDraw()
{
	SDL_Renderer* renderer = Graphics_GetActiveRenderer();

	Text *pressStart;
	
	Vec2d startTextPos;

	int random;

	const Uint8 *keys;
	SDL_Event e;

	int done;

	font = TTF_OpenFont("fonts/menu.ttf", 36);
	font2 = TTF_OpenFont("fonts/menu.ttf", 18);

	if(font == NULL)return;
	if(font2 == NULL)return;

	done = 0;

	pressStart = sprite_LoadText(font, "Press Enter to Start", textColor);
	bg = sprite_Load("images/menu2.png",1000,703);
	
	vec2d_Set(bgPos,0,0);
	vec2d_Set(startTextPos,320,500);

	sprite_Draw(bg,0,renderer,bgPos);


	do 
	{	
		random = rand() % 150+100;	
		sprite_TextDraw(pressStart,startTextPos, random);
		
		SDL_RenderPresent(renderer);
		SDL_PollEvent (&e);
		if(&e)
		{
			if( e.type == SDL_KEYUP)
			{
				//Select Surface based on key presses
				switch(e.key.keysym.sym)
				{
				case SDLK_UP:
				case SDLK_w:
					e.type = SDLK_CLEAR;
					break;

				case SDLK_DOWN:
				case SDLK_s:
					e.type = SDLK_CLEAR;
					break;
				case SDLK_RETURN:
				case SDLK_SPACE:
					done = 1; 
					e.type = SDLK_CLEAR;
					break;

				}
			}
		}
	keys = SDL_GetKeyboardState(NULL);
	if(keys[SDL_SCANCODE_ESCAPE])
		{
			exit(1);
		}

	}
	while(!done);
	return;
}

void Menu_MainDraw()
{
	SDL_Renderer* renderer = Graphics_GetActiveRenderer();

	const Uint8 *keys;
	SDL_Event e;

	int done;
	int select = 0;

	Text *NewGame;
	Text *LoadGame;
	Text *QuitGame;

	Vec2d NGPos;
	Vec2d LGPos;
	Vec2d QuitPos;

	Vec3d color;

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	if(font == NULL)return;

	NewGame = sprite_LoadText(font,"New Game", textColor);
	LoadGame = sprite_LoadText(font,"Load Game", textColor);
	QuitGame = sprite_LoadText(font,"Quit Game", textColor);

	vec2d_Set(NGPos, 380, 500);
	vec2d_Set(LGPos, 380, 550);
	vec2d_Set(QuitPos, 380, 600);

	vec3d_Set(color, 254,254,254);

	sprite_Draw(bg,0,renderer, bgPos);

	sprite_BloomDraw(NewGame,0,renderer,NGPos, color, 5);
	sprite_TextDraw(NewGame, NGPos, 255);

	sprite_BloomDraw(LoadGame,0,renderer,LGPos, color, 5);
	sprite_TextDraw(LoadGame, LGPos, 255);

	sprite_BloomDraw(QuitGame,0,renderer,QuitPos, color, 5);
	sprite_TextDraw(QuitGame, QuitPos, 255);

	done = 0;

	do
	{
		sprite_TextDraw(NewGame, NGPos, 255);
		sprite_TextDraw(LoadGame, LGPos, 255);
		sprite_TextDraw(QuitGame, QuitPos, 255);

		SDL_RenderPresent(renderer);
		switch(select)
		{
			case 0:
				//sprite_BloomDraw(NewGame,0,renderer,NGPos, color, 5);
				
				//sprite_TextDraw(LoadGame, LGPos);
				//sprite_TextDraw(QuitGame, QuitPos);
				SDL_SetTextureColorMod(NewGame->image,255,255,255);

				SDL_SetTextureColorMod(LoadGame->image, 28,1,13);
				SDL_SetTextureColorMod(QuitGame->image, 28,1,13);
			break;

			case 1:
				//sprite_BloomDraw(LoadGame,0,renderer,LGPos, color, 5);

				//sprite_TextDraw(NewGame, NGPos);
				//sprite_TextDraw(QuitGame, QuitPos);

				SDL_SetTextureColorMod(LoadGame->image,255,255,255);

				SDL_SetTextureColorMod(NewGame->image, 28,1,13);
				SDL_SetTextureColorMod(QuitGame->image, 28,1,13);
				break;
		
			case 2:
				//sprite_BloomDraw(QuitGame,0,renderer,QuitPos, color, 5);

				//sprite_TextDraw(NewGame, NGPos);
				//sprite_TextDraw(LoadGame, LGPos);
				SDL_SetTextureColorMod(QuitGame->image,255,255,255);

				SDL_SetTextureColorMod(NewGame->image, 28,1,13);
				SDL_SetTextureColorMod(LoadGame->image, 28,1,13);
				break;
		}

		SDL_PollEvent (&e);
		
		if(&e)
		{
			if(e.type == SDL_KEYUP)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_UP:
					case SDLK_w:
						if(select > 0)
						{
							select--;
						}
						e.type = SDLK_CLEAR;
						break;

					case SDLK_DOWN:
					case SDLK_s:
						if(select < 2)
						{
							select++;
						}
						e.type = SDLK_CLEAR;
						break;

					case SDLK_RETURN:
					case SDLK_SPACE:
						switch(select)
						{
						case 0:
							Level_Load(0, 0);
							done = 1;
							break;
						case 1:
		
							Level_Load(0, 1);
							done = 1;
							break;
						case 2:
							exit(1);
							break;
						}
						break;
					}
			}
		}
		keys = SDL_GetKeyboardState(NULL);
		
		if(keys[SDL_SCANCODE_ESCAPE])
		{
			exit(1);
		}
	}
	while(!done);		

	return;
}