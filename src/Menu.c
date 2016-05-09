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
#include "Player.h"

TTF_Font *font = NULL;
TTF_Font *font2 = NULL;

SDL_Color textColor = { 33, 3, 0 };
SDL_Color textColor2 = {80, 80, 80};

Sprite *bg;
Vec2d bgPos;

extern Entity *p;
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
	font2 = TTF_OpenFont("fonts/menu.ttf", 48);

	if(font == NULL)return;
	if(font2 == NULL)return;

	done = 0;

	pressStart = sprite_LoadText(font, "Press Enter to Start", textColor);
	bg = sprite_Load("images/menu2.png",1000,703);
	
	vec2d_Set(bgPos,0,0);
	vec2d_Set(startTextPos,320,500);

	sprite_Draw(bg,0,renderer,bgPos,1);


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

	sprite_Draw(bg,0,renderer, bgPos,1);

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
				SDL_SetTextureColorMod(NewGame->image,255,255,255);

				SDL_SetTextureColorMod(LoadGame->image, 28,1,13);
				SDL_SetTextureColorMod(QuitGame->image, 28,1,13);
			break;

			case 1:
				SDL_SetTextureColorMod(LoadGame->image,255,255,255);

				SDL_SetTextureColorMod(NewGame->image, 28,1,13);
				SDL_SetTextureColorMod(QuitGame->image, 28,1,13);
				break;
		
			case 2:
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
							Menu_CharacterEditor();
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
void Menu_CharacterEditor()
{
	SDL_Renderer* renderer = Graphics_GetActiveRenderer();
	
	CSprite *player;
	const Uint8 *keys;
	SDL_Event e;

	int done;
	int select=0;
	int hselect=0,fselect=0,sselect=0,jselect=0,aselect=0;

	int hf=0,ff=0,sf=0,jf=0,af=0;

	Text *CharacterEditor;
	Text *Head;
	Text *Face;
	Text *Shirt;
	Text *Jacket;
	Text *Accessory;

	Vec2d initPos;

	Vec2d cspritePos;

	Vec2d ChEdPos;
	Vec2d HeadPos;
	Vec2d FacePos;
	Vec2d ShirtPos;
	Vec2d JacketPos;
	Vec2d AccPos;

	Vec3d color;

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	if(font == NULL)return;

	p = Player_Load(-1700+468,0+325);

	player = csprite_Load("images/hairsheet.png","images/facesheet.png","images/shirtsheet.png","images/jacketsheet.png");

	CharacterEditor = sprite_LoadText(font, "Character Editor", textColor);
	Head = sprite_LoadText(font,"Head", textColor2);
	Face = sprite_LoadText(font,"Face", textColor2);
	Shirt = sprite_LoadText(font,"Shirt", textColor2);
	Jacket = sprite_LoadText(font,"Jacket", textColor2);

	vec2d_Set(cspritePos, 460,300);
	
	vec2d_Set(ChEdPos, 50, 50);
	vec2d_Set(HeadPos, 70, 200);
	vec2d_Set(FacePos, 70, 300);
	vec2d_Set(ShirtPos, 70, 400);
	vec2d_Set(JacketPos, 70, 500);
	vec2d_Set(AccPos, 70, 600);

	vec3d_Set(color, 254,254,254);
	bg = sprite_Load("images/charactereditor.png",1000,703);
	sprite_Draw(bg,0,renderer, bgPos,1);

	sprite_BloomDraw(Head,0,renderer,HeadPos, color, 5);
	sprite_TextDraw(Head, HeadPos, 255);

	sprite_BloomDraw(Face,0,renderer,FacePos, color, 5);
	sprite_TextDraw(Face, FacePos, 255);

	sprite_BloomDraw(Shirt,0,renderer,ShirtPos, color, 5);
	sprite_TextDraw(Shirt, ShirtPos, 255);

	sprite_BloomDraw(Jacket,0,renderer,JacketPos, color, 5);
	sprite_TextDraw(Jacket, JacketPos, 255);

	done = 0;

	do
	{
		sprite_Draw(bg,0,renderer, bgPos,1);
		sprite_TextDraw(Head, HeadPos, 255);
		sprite_TextDraw(Face, FacePos, 255);
		sprite_TextDraw(Shirt, ShirtPos, 255);
		sprite_TextDraw(Jacket, JacketPos, 255);
		csprite_Draw(player,hf,ff,sf,jf,renderer, cspritePos,3);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		switch(select)
		{
			case 0:
				SDL_SetTextureColorMod(Head->image,255,255,255);

				SDL_SetTextureColorMod(Face->image, 28,1,13);
				SDL_SetTextureColorMod(Shirt->image, 28,1,13);
				SDL_SetTextureColorMod(Jacket->image, 28,1,13);
//				SDL_SetTextureColorMod(Accessory->image, 28,1,13);
			break;

			case 1:
				SDL_SetTextureColorMod(Face->image,255,255,255);

				SDL_SetTextureColorMod(Head->image, 28,1,13);
				SDL_SetTextureColorMod(Shirt->image, 28,1,13);
				SDL_SetTextureColorMod(Jacket->image, 28,1,13);
//				SDL_SetTextureColorMod(Accessory->image, 28,1,13);
				break;
		
			case 2:
				SDL_SetTextureColorMod(Shirt->image,255,255,255);

				SDL_SetTextureColorMod(Face->image, 28,1,13);
				SDL_SetTextureColorMod(Head->image, 28,1,13);
				SDL_SetTextureColorMod(Jacket->image, 28,1,13);
//				SDL_SetTextureColorMod(Accessory->image, 28,1,13);
				break;
			case 3:
				SDL_SetTextureColorMod(Jacket->image,255,255,255);

				SDL_SetTextureColorMod(Face->image, 28,1,13);
				SDL_SetTextureColorMod(Shirt->image, 28,1,13);
				SDL_SetTextureColorMod(Head->image, 28,1,13);
//				SDL_SetTextureColorMod(Accessory->image, 28,1,13);
				break;
			/*case 4:
				SDL_SetTextureColorMod(Accessory->image,255,255,255);

				SDL_SetTextureColorMod(Face->image, 28,1,13);
				SDL_SetTextureColorMod(Shirt->image, 28,1,13);
				SDL_SetTextureColorMod(Jacket->image, 28,1,13);
				SDL_SetTextureColorMod(Head->image, 28,1,13);
				break;*/
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
					if(select < 3)
					{
						select++;
					}
					e.type = SDLK_CLEAR;
					break;

				case SDLK_LEFT:
				case SDLK_a:
					if(hselect > 0)
					{
						hselect--;
					}
					e.type = SDLK_CLEAR;
					break;

				case SDLK_RIGHT:
				case SDLK_d:
					if(hselect < 3)
					{
						hselect++;
					}
					e.type = SDLK_CLEAR;
					break;
				case SDLK_RETURN:
					
					p->csprite = CSprite_Maker(hf,ff,sf,jf);
					done = 1;
				}
			
				switch(select)
				{
				case 0:
					hf = hselect;
					break;
				case 1:
					ff = hselect;
					break;
				case 2:
					sf = hselect;
					break;
				case 3:
					jf = hselect;
					break;
				/*case 4:
					af = hselect;
					break;*/
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
CSprite *CSprite_Maker(int hair, int face, int shirt, int jacket)
{
	CSprite *temp;

	char *hairfile;
	char *facefile;
	char *shirtfile;
	char *jacketfile;

	switch(hair)
	{
	case 0:
		hairfile = "images/playersheet/redhair.png";
		break;
	case 1:
		hairfile = "images/playersheet/yellowhair.png";
		break;
	case 2:
		hairfile = "images/playersheet/brownhair.png";
		break;
	case 3:
		hairfile = "images/playersheet/whitehair.png";
		break;
	}

	switch(face)
	{
	case 0:
		facefile = "images/playersheet/whiteface.png";
		break;
	case 1:
		facefile = "images/playersheet/redface.png";
		break;
	case 2:
		facefile = "images/playersheet/blackface.png";
		break;
	case 3:
		facefile = "images/playersheet/yellowface.png";
		break;
	}

	switch(shirt)
	{
	case 0:
		shirtfile = "images/playersheet/greenshirt.png";
		break;
	case 1:
		shirtfile = "images/playersheet/blueshirt.png";
		break;
	case 2:
		shirtfile = "images/playersheet/redshirt.png";
		break;
	case 3:
		shirtfile = "images/playersheet/purpleshirt.png";
		break;
	}

	switch(jacket)
	{
	case 0:
		jacketfile = "images/playersheet/greenjack.png";
		break;
	case 1:
		jacketfile = "images/playersheet/tealjack.png";
		break;
	case 2:
		jacketfile = "images/playersheet/redjack.png";
		break;
	case 3:
		jacketfile = "images/playersheet/blackjack.png";
		break;
	}

	temp = csprite_Load(hairfile,facefile,shirtfile,jacketfile);
	return temp;
}