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

static int gametime;
void Init_All();
void Update_All();
TTF_Font *font2 =  NULL;
TTF_Font *font =  NULL;
HUD *hud;
/**
 * @brief	Main entry-point for this application.
 * @param	argc	Number of command-line arguments.
 * @param	argv	Array of command-line argument strings.
 * @return	Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char *argv[])
{
	int done;
	const Uint8 *keys;
	

	if( TTF_Init() == -1)
    {
        return false;    
    }
    
	font2 = TTF_OpenFont( "fonts/font1.ttf", 36 );
	
	font = TTF_OpenFont( "fonts/font.ttf", 36 );
	if( font == NULL )
    {
		slog("font not loaded properly");
		return false;
    }

	Init_All();
	
	done = 0;
	do
	{
		gametime = SDL_GetTicks();
		Update_All();
		
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
	Level_Load(0);
	hud = InitHUD();
}
/** @brief	Updates all every frame */
void Update_All()
{
		SDL_RenderClear(Graphics_GetActiveRenderer());

		Room_DrawAll();
		Entity_UpdateAll();
		DrawGui(hud);
		NextFrame();		
		SDL_PumpEvents();
}
