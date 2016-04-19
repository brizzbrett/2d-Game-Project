#include <stdlib.h>
#include <string>
#include <cmath>

#include "SDL.h"
#include "SDL_image.h"

#include "graphics.h"
#include "sprite.h"
#include "Audio.h"
#include "Vector.h"

#include "Camera.h"
#include "Level.h"

void Init_All();
void Update_All();
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

	Init_All();
	
	done = 0;
	do
	{
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
	Entity_InitSystem(1000);
	Graphics_Init("Dream a Way Out",1600,900,0);
	Level_Load(0);
}
/** @brief	Updates all every frame */
void Update_All()
{
		SDL_RenderClear(Graphics_GetActiveRenderer());

		Room_DrawAll();
		Entity_UpdateAll();
		
		NextFrame();		
		SDL_PumpEvents();
}
