#include <stdlib.h>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "graphics.h"
#include "sprite.h"
#include "Vector.h"

#include "Camera.h"
#include "Level.h"

#include "Entity.h"
#include "Player.h"
#include "Enemy_Glop.h"
#include "Enemy_Eye.h"

extern SDL_Surface *screen; /**<pointer to the draw screen*/
extern SDL_Surface *buffer; /**<pointer to the draw buffer*/


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
	int i = 0;
	Player *player;
	Glop *glop = NULL;
	Glop *glop2 = NULL;
	Glop *glop4 = NULL;
	Glop *glop3 = NULL;
	Eye *eye = NULL;
	Eye *eye2 = NULL;
	Eye *eye3 = NULL;
	Eye *eye4 = NULL;
	Eye *eye5 = NULL;

	Init_All();

	temp = IMG_Load("images/bgtest.png");
	if(temp != NULL)
	{
		fprintf(stdout, "temp image successfully loaded\n");
		SDL_BlitSurface(temp,NULL,buffer,NULL);
	}

	player = Player_Load(775,600);
	glop = Glop_Load(1,100,100);
	glop2 = Glop_Load(2,100,800);
	glop3 = Glop_Load(3,1500,100);
	glop4 = Glop_Load(4, 1500,800);
	eye = Eye_Load(5,775,100);
	eye2 = Eye_Load(6,775,800);
	eye3 = Eye_Load(7,100,450);
	eye4 = Eye_Load(8,1500,450);
	eye5 = Eye_Load(8,775,450);
	done = 0;
	do
	{
		SDL_RenderClear(Graphics_GetActiveRenderer());
		
		Graphics_RenderSurfaceToScreen(temp,Camera_GetActiveCamera(),Camera_GetPosition().x,Camera_GetPosition().y);

		Room_DrawAll();

		Entity_ThinkAll();
		Entity_UpdateAll();
		Camera_IntersectAll(player);
		Entity_IntersectAll(player);	

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
	Entity_InitSystem(1000);
	Node_InitSystem();
	Graphics_Init("Dream a Way Out",1600,900,1600,900,bgcolor,0);
	Level_Load();
	LinkedRooms();
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
