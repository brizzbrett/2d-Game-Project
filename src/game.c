#include <stdlib.h>
#include <string>
#include <cmath>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "graphics.h"
#include "sprite.h"
#include "Audio.h"
#include "Vector.h"

#include "Camera.h"
#include "Level.h"
#include "HUD.h"
#include "Menu.h"

static int gametime;
void Init_All();
void Update_Game();
//TTF_Font *font2 =  NULL;
//TTF_Font *font =  NULL;
HUD *hud;	
//Menu *menu;
/**
 * @brief	Main entry-point for this application.
 * @param	argc	Number of command-line arguments.
 * @param	argv	Array of command-line argument strings.
 * @return	Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char *argv[])
{
	int done;
	int menuPress;
	const Uint8 *keys;


	if( TTF_Init() == -1)
    {
        return false;    
    }

	Init_All();
	menuPress = 0;
	done = 0;
	do
	{
		SDL_RenderClear(Graphics_GetActiveRenderer());

		Update_Game();
		menuPress = 1;
	
		NextFrame();		
		SDL_PumpEvents();
		keys = SDL_GetKeyboardState(NULL);
		if(keys[SDL_SCANCODE_ESCAPE])done = 1;
	}while(!done);

	exit(0);
	return 0;
}
/** @brief	Initialises all. */
void Init_All()
{
	sprite_InitSystem();
	Audio_InitSystem(128,3);
	Entity_InitSystem(10000);
	
	Graphics_Init("Dream a Way Out",1000,703,0);
	Menu_TitleScreenDraw();
	Menu_MainDraw();
	hud = HUD_Init();
}
/** @brief	Updates all every frame */
void Update_Game()
{
	Room_DrawAll();
	Entity_UpdateAll();
	HUD_Draw(hud);
}
