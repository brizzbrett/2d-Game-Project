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
static int maxRooms = 14;
static GList *deadEnds;

Level *Level_New(char *file)
{
	Level *l;
	
	l = (Level *)malloc(sizeof(Level));

	l->data = NULL;
	l->f = fopen(file, "rb");
	l->len = NULL;
	l->bk_music = NULL;

	return l;

}
void Level_Load(Uint8 levelType)
{
	Room *r;
	GList *g;
	currentLevel = Level_New(levelcfg);
	cJSON *json, *root, *obj;

	srand(time(NULL));
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
		Level_Maker("images/nightmare.png", NIGHTMARE);	
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		Room_Populate(NIGHTMARE);
	}
	else if(levelType == COWBOY)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		Level_Maker("images/cowboy.png", COWBOY);

		Room_Populate(COWBOY);
	}
	else if(levelType == FUTURE)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		Level_Maker("images/room.png", FUTURE);

		Room_Populate(NIGHTMARE);
	}
	else if(levelType == MEDIEVAL)
	{
		currentLevel->bk_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(currentLevel->bk_music);

		Level_Maker("images/room.png", MEDIEVAL);

		Room_Populate(NIGHTMARE);
	}		
	slog("ALL ROOMS");
	for (g = RoomList_Get(); g != NULL; g = g->next)
	{
		r = (Room *)(g->data);

		slog("\nROOM %c-------------\n\tPOSITION-- X: %.00f, Y: %.00f, Doors: %i, Type: %i", r->val+65, r->pos.x, r->pos.y, r->doors, r->type);
	}
	slog("\n\nDEAD ENDS");
	for (g = DeadEnds_Get(); g != NULL; g = g->next)
	{
		r = (Room *)(g->data);
		
		slog("\nROOM %c-------------\n\tPOSITION-- X: %.00f, Y: %.00f, Doors: %i, Type: %i", r->val+65, r->pos.x, r->pos.y, r->doors, r->type);
	}
}
void Level_Closer(int level)
{
	GList *it;
	Room *rit;
	Entity_FreeByLevel(level);
	Music_Free(&currentLevel->bk_music);
	g_list_free_full(deadEnds, g_free);
	deadEnds = NULL;
	Room_FreeByLevel(level);
}
Room *Room_GetByPosition(int x, int y)
{
	Room *rit;
	GList *it;

	for (it = RoomList_Get(); it != NULL; it = it->next)
	{
		rit = (Room *)(it->data);
		if(rit->pos.x == x && rit->pos.y == y)
		{
			return rit;
		}
	}
	return NULL;
}
void Level_Maker(char *file, int levelin)
{
	Room *currRoom;
	Room *newRoom;
	Room *tempRoom;
	Room *rit;
	GList *it;

	int randPath;
	int i;
	Vec2d pos;
	if(levelin == 1)
	{
		vec2d_Set(pos,-25600,-14400);
	}
	else if(levelin == 2)
	{
		vec2d_Set(pos,-25600,14400);
	}
	else
	{
		vec2d_Set(pos,25600, -14400);
	}
	currRoom = Room_New(pos, file, levelin);
	for(i = 0; i < maxRooms; i++)
	{
		randPath = rand() % 4;
		if(randPath == 0)
		{
			tempRoom = Room_GetByPosition(currRoom->pos.x, currRoom->pos.y-ROOM_HEIGHT-500);
			if(currRoom->nroom)
			{
				currRoom = currRoom->nroom;
				i--;
			}
			else if(tempRoom)
			{
				Room_Link(tempRoom, currRoom, SPLIT_HORIZONTAL, levelin);
				currRoom->nroom = tempRoom;
				tempRoom->sroom = currRoom;
				currRoom = tempRoom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x,currRoom->pos.y-ROOM_HEIGHT-500);
				newRoom = Room_New(pos, file, levelin);
				currRoom->nroom = newRoom;
				newRoom->sroom = currRoom;
				Room_Link(currRoom,newRoom, SPLIT_HORIZONTAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
		else if(randPath == 1)
		{
			tempRoom = Room_GetByPosition(currRoom->pos.x, currRoom->pos.y+ROOM_HEIGHT+500);
			if(currRoom->sroom)
			{
				currRoom = currRoom->sroom;
				i--;
			}
			else if(tempRoom)
			{
				Room_Link(currRoom, tempRoom, SPLIT_HORIZONTAL, levelin);
				currRoom->nroom = tempRoom;
				tempRoom->sroom = currRoom;
				currRoom = tempRoom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x,currRoom->pos.y+ROOM_HEIGHT+500);
				newRoom = Room_New(pos, file, levelin);
				currRoom->sroom = newRoom;
				newRoom->nroom = currRoom;
				Room_Link(newRoom,currRoom, SPLIT_HORIZONTAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
		else if(randPath == 2)
		{
			tempRoom = Room_GetByPosition(currRoom->pos.x-ROOM_WIDTH-500,currRoom->pos.y);
			if(currRoom->wroom)
			{
				currRoom = currRoom->wroom;
				i--;
			}
			else if(tempRoom)
			{
				Room_Link(tempRoom, currRoom, SPLIT_VERTICAL, levelin);
				currRoom->wroom = tempRoom;
				tempRoom->eroom = currRoom;
				currRoom = tempRoom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x-ROOM_WIDTH-500,currRoom->pos.y);
				newRoom = Room_New(pos, file, levelin);
				currRoom->wroom = newRoom;
				newRoom->eroom = currRoom;
				Room_Link(newRoom,currRoom, SPLIT_VERTICAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
		else
		{
			tempRoom = Room_GetByPosition(currRoom->pos.x+ROOM_WIDTH+500,currRoom->pos.y);
			if(currRoom->eroom)
			{
				currRoom = currRoom->eroom;
				i--;
			}
			else if(tempRoom)
			{
				Room_Link(tempRoom, currRoom, SPLIT_VERTICAL, levelin);
				currRoom->eroom = tempRoom;
				tempRoom->wroom = currRoom;
				currRoom = tempRoom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x+ROOM_WIDTH+500,currRoom->pos.y);
				newRoom = Room_New(pos, file, levelin);
				currRoom->eroom = newRoom;
				newRoom->wroom = currRoom;
				Room_Link(newRoom, currRoom, SPLIT_VERTICAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
	}

	for (it = RoomList_Get(); it != NULL; it = it->next)
	{
		rit = (Room *)(it->data);
		if(rit->doors == 1)
		{
			if(rit->type == RTYPE_HUBR || rit->type == RTYPE_HUBH || rit->type == RTYPE_HUBM)
			{
				continue;
			}
			else
			{
				deadEnds = g_list_append(deadEnds, rit);
			}
		}

	}
	WinPath();
}

GList *DeadEnds_Get()
{
	return deadEnds;
}