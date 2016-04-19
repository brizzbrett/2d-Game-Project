#include "Level.h"
#include <cmath>
#include <time.h>
#include <random>

#define HUB 0
#define NIGHTMARE 1
#define COWBOY 2
#define FUTURE 3
#define MEDIEVAL 4

Level *currentLevel;
static char *levelcfg = "def/levelcfg.txt";

Level *Level_New(char *file)
{
	Level *l;
	l = (Level *)malloc(sizeof(Level));

	l->g = NULL;
	l->r = NULL;
	l->data = NULL;
	l->f = fopen(file, "rb");
	l->len = NULL;
	l->bk_music = NULL;

	return l;

}
void Level_Load(Uint8 levelType)
{
	currentLevel = Level_New(levelcfg);
	cJSON *json, *root, *obj;

	if(!levelcfg)return;

	if(!currentLevel->f)return;

	fseek(currentLevel->f,0,SEEK_END);
	currentLevel->len=ftell(currentLevel->f);
	fseek(currentLevel->f,0,SEEK_SET);

	currentLevel->data=(char*)malloc(currentLevel->len+1);
	fread(currentLevel->data,1,currentLevel->len,currentLevel->f);
	fclose(currentLevel->f);

	json = cJSON_Parse(currentLevel->data);
	if(!json)return;
	if(levelType == HUB)
	{
		Hub_Create(levelcfg);
	}
	else if(levelType == NIGHTMARE)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		srand(time(NULL));
		Level_Maker("images/room.png", NIGHTMARE);
	}
	else if(levelType == COWBOY)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		srand(time(NULL));
		Level_Maker("images/room.png", COWBOY);
	}
	else if(levelType == FUTURE)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		srand(time(NULL));
		Level_Maker("images/room.png", FUTURE);
	}
	else if(levelType == MEDIEVAL)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		srand(time(NULL));
		Level_Maker("images/room.png", MEDIEVAL);
	}
}
void Level_Closer(int level)
{
	Entity_FreeByLevel(level);
	Music_Free(&currentLevel->bk_music);
	//Room_FreeByLevel(level);
}