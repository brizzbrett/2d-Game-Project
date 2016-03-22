#include <stdlib.h>
#include <string>
#include <cmath>

#include "SDL.h"
#include "SDL_image.h"

#include "graphics.h"
#include "sprite.h"
#include "Vector.h"

#include "Camera.h"
#include "Level.h"

extern SDL_Surface *buffer; /**<pointer to the draw buffer*/

void Init_All();

/**
 * @brief	Main entry-point for this application.
 * @param	argc	Number of command-line arguments.
 * @param	argv	Array of command-line argument strings.
 * @return	Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char *argv[])
{
	SDL_Surface *temp = NULL;
	int done;
	const Uint8 *keys;

	Init_All();
	
	Level_Load();

	temp = IMG_Load("images/bgtest.png");
	if(temp != NULL)
	{
		fprintf(stdout, "temp image successfully loaded\n");
		SDL_BlitSurface(temp,NULL,buffer,NULL);
	}

	done = 0;
	do
	{
		SDL_RenderClear(Graphics_GetActiveRenderer());

		Room_DrawAll();
		Entity_UpdateAll();
		
		NextFrame();
		SDL_PumpEvents();
		keys = SDL_GetKeyboardState(NULL);
		if(keys[SDL_SCANCODE_ESCAPE])done = 1;
	}while(!done);

	SDL_FreeSurface(temp);

	exit(0);
	return 0;
}

/** @brief	Initialises all. */
void Init_All()
{
	sprite_InitSystem();
	Entity_InitSystem(1000);
	Node_InitSystem();
	Graphics_Init("Dream a Way Out",1600,900,0);
}
