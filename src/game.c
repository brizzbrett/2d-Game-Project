#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "sprite.h"
#include "Vector.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy_Glop.h"
#include "Camera.h"
#include "Level.h"
#include <string>

extern SDL_Surface *screen; /**<pointer to the draw screen*/
extern SDL_Surface *buffer; /**<pointer to the draw buffer*/
extern SDL_Rect Camera; /**<Camera*/

void Init_All();

int getImagePathFromFile(char *filepath,char * filename);
int getCoordinatesFromFile(int *x, int *y,char * filename);
void addCoordinateToFile(char *filepath,int x, int y);

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
	int tx = 0,ty = 0;
	const Uint8 *keys;
	char imagepath[512];
	SDL_Rect srcRect={0,0,1600,800};
	int i = 0;
	Player *player;
	Player *player2;
	Glop *glop;

	Init_All();

	temp = IMG_Load("images/bgtest.png");
	if(temp != NULL)
	{
		fprintf(stdout, "temp image successfully loaded\n");
		SDL_BlitSurface(temp,NULL,buffer,NULL);
	}

	player = Player_Load();
	glop = Glop_Load();
	done = 0;
	do
	{
		SDL_RenderClear(Graphics_GetActiveRenderer());

		if(You_Died())
		{
			temp = IMG_Load("images/youdied.png");

			SDL_BlitSurface(temp,NULL,buffer,NULL);

			Graphics_RenderSurfaceToScreen(temp,srcRect,0,0);
			Entity_Free(&player);
			Entity_Free(&glop);
		}
		else
		{
			Graphics_RenderSurfaceToScreen(temp,srcRect,0,0);
		}

		Entity_ThinkAll();
		Entity_UpdateAll();
		Entity_DrawAll();
		Entity_IntersectAll(player);

		ResetBuffer();
		NextFrame();
		SDL_PumpEvents();
		keys = SDL_GetKeyboardState(NULL);
		if(keys[SDL_SCANCODE_ESCAPE])done = 1;
	}while(!done);

	SDL_FreeSurface(temp);

	exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
	return 0;
}

/** @brief	Initialises all. */
void Init_All()
{
	float bgcolor[] = {1,1,1,1};
	sprite_InitSystem();
	Entity_InitSystem(100);
	Graphics_Init("Dream a Way Out",1600,800,1600,800,bgcolor,0);
	Level_Load();
}

int getImagePathFromFile(char *filepath,char * filename)
{
    FILE *fileptr = NULL;
    char buf[255];
    int returnValue = -1;
    if (!filepath)
    {
        fprintf(stdout,"getImagePathFromFile: warning, no output parameter provided\n");
        return -1;
    }
    if (!filename)
    {
        fprintf(stdout,"getImagePathFromFile: warning, no input file path provided\n");
        return -1;
    }
    fileptr = fopen(filename,"r");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filename);
        return -1;
    }
    if (fscanf(fileptr,"%s",buf))
    {
        if (strcmp(buf,"image:")==0)
        {
            fscanf(fileptr,"%s",filepath);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    return returnValue;
}

void addCoordinateToFile(char *filepath,int x, int y)
{
    FILE *fileptr = NULL;
    if (!filepath)
    {
        fprintf(stdout,"addCoordinateToFile: warning, no input file path provided\n");
        return;
    }
    fileptr = fopen(filepath,"a");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filepath);
        return;
    }
    fprintf(fileptr,"%s:%i:newcoordinate: %i %i\n",__FILE__,__LINE__,x,y);
    fclose(fileptr);
}

int getCoordinatesFromFile(int *x, int *y,char * filename)
{
    FILE *fileptr = NULL;
    char buf[255];
    int tx,ty;
    int returnValue = -1;
    if ((!x)&&(!y))
    {
        fprintf(stdout,"getCoordinatesFromFile: warning, no output parameter provided\n");
        return -1;
    }
    if (!filename)
    {
        fprintf(stdout,"getCoordinatesFromFile: warning, no input file path provided\n");
        return -1;
    }
    fileptr = fopen(filename,"r");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filename);
        return -1;
    }
    while (fscanf(fileptr,"%s",buf) != EOF)
    {
        fprintf(stdout,"buf is: %s\n",buf);
        if (strcmp(buf,"position:")==0)
        {
            fscanf(fileptr,"%i %i",&tx,&ty);
            fprintf(stdout,"as read: %i, %i\n",tx,ty);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    if (x)*x = tx;
    if (y)*y = ty;
    return returnValue;
}
