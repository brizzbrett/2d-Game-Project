#include "Level.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>

#include "Player.h"
#include "Items.h"

void Hub_Create(char *file, int save)
{
	Room *r[11];
	char *data;
	FILE *f;
	long len;

	FILE *saveFile;
	char *savedata;
	int savelen;
	Vec2d tVec;
	bool door[12];
	cJSON *json, *root, *obj, *room, *buf;
	cJSON *save_json, *save_root;
	Vec2d bed;
	int nightbed=0,cowbed=0,futbed=0,medbed=0;

	f = fopen(file, "rb");

	if(!f)return;

	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);

	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);

	if(save)
	{
		saveFile = fopen("def/savecfg.txt", "rb");
		if(!saveFile)return;

		fseek(saveFile,0,SEEK_END);
		savelen=ftell(saveFile);
		fseek(saveFile,0,SEEK_SET);

		savedata=(char*)malloc(savelen+1);
		fread(savedata,1,savelen,saveFile);
		fclose(saveFile);

		save_json = cJSON_Parse(savedata);
		save_root = cJSON_GetObjectItem(save_json, "save");
		nightbed = cJSON_GetObjectItem(save_root, "nightbed")->valueint;
		cowbed = cJSON_GetObjectItem(save_root, "cowbed")->valueint;
		futbed = cJSON_GetObjectItem(save_root, "futbed")->valueint;
		medbed = cJSON_GetObjectItem(save_root, "medbed")->valueint;
	}
	
	json = cJSON_Parse(data);
	if(!json)return;
	root = cJSON_GetObjectItem(json, "HUB");
	if(!root)return;
	obj = cJSON_GetObjectItem(root, "level load");
	if(!obj)return;
	room = cJSON_GetObjectItem(obj, "base room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[0] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[0]->type = RTYPE_HUBR;
	vec2d_Set(bed,r[0]->pos.x+150,r[0]->pos.y+400);
	Item_Spawn(Bed_New(bed,1), 0);
	if(nightbed == 1)
	{
		Bed_GetByBedLevel(1)->touch = NULL;
		Bed_GetByBedLevel(1)->flag = 1;
	}
	room = cJSON_GetObjectItem(obj, "hall1");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[1] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[1]->type = RTYPE_HUBH;

	room = cJSON_GetObjectItem(obj, "hall2");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[2] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[2]->type = RTYPE_HUBH;

	room = cJSON_GetObjectItem(obj, "hall3");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[3] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[3]->type = RTYPE_HUBH;

	room = cJSON_GetObjectItem(obj, "hall4");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[4] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[4]->type = RTYPE_HUBH;

	room = cJSON_GetObjectItem(obj, "cowboy room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[5] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[5]->type = RTYPE_HUBR;
	vec2d_Set(bed,r[5]->pos.x+150,r[5]->pos.y+400);
	Item_Spawn(Bed_New(bed,3), 0);
	if(cowbed == 1)
	{
		Bed_GetByBedLevel(3)->touch = NULL;
		Bed_GetByBedLevel(3)->flag = 1;
	}

	room = cJSON_GetObjectItem(obj, "future room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[6] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[6]->type = RTYPE_HUBR;
	vec2d_Set(bed,r[6]->pos.x+150,r[6]->pos.y+400);
	Item_Spawn(Bed_New(bed,2), 0);
	if(futbed == 1)
	{
		Bed_GetByBedLevel(2)->touch = NULL;
		Bed_GetByBedLevel(2)->flag = 1;
	}

	room = cJSON_GetObjectItem(obj, "medieval room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[7] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[7]->type = RTYPE_HUBR;
	vec2d_Set(bed,r[7]->pos.x+150,r[7]->pos.y+400);
	Item_Spawn(Bed_New(bed,4), 0);
	if(medbed == 1)
	{
		Bed_GetByBedLevel(4)->touch = NULL;
		Bed_GetByBedLevel(4)->flag = 1;
	}

	room = cJSON_GetObjectItem(obj, "final room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[8] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[8]->type = RTYPE_HUBM;
	room = cJSON_GetObjectItem(obj, "bath room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[9] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[9]->type = RTYPE_HUBM;
	room = cJSON_GetObjectItem(obj, "kitchen");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[10] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, NULL);
	r[10]->type = RTYPE_HUBM;

	Room_Link(r[1],r[0],SPLIT_HORIZONTAL, NULL);/**<Hall 1 to Base Room*/
	Room_Link(r[2],r[1],SPLIT_HORIZONTAL, NULL);/**<Hall 2 to Hall 1*/
	Room_Link(r[1],r[5],SPLIT_VERTICAL, NULL);/**<Cowboy Room to Hall 1*/
	Room_Link(r[6],r[1],SPLIT_VERTICAL, NULL);/**<Hall 1 to Future Room*/
	Room_Link(r[2],r[9],SPLIT_VERTICAL, NULL);/**<Hall 2 to Bath Room*/
	Room_Link(r[3],r[2],SPLIT_HORIZONTAL, NULL);/**<Hall 3 to Hall 2*/
	Room_Link(r[10],r[3],SPLIT_VERTICAL, NULL);/**<Kitchen to Hall 3*/
	Room_Link(r[8],r[10],SPLIT_HORIZONTAL, NULL);/**<Final Room to Kitchen*/
	Room_Link(r[4],r[3],SPLIT_HORIZONTAL, NULL);/**<Hall 4 to Hall 3*/
	Room_Link(r[8],r[4],SPLIT_VERTICAL, NULL);/**<Final Room to Hall 4*/
	Room_Link(r[7],r[4],SPLIT_HORIZONTAL, NULL);/**<Medieval Room to Hall 4*/

	free(data);
	json = NULL;
	root = NULL;
	obj = NULL;
	room = NULL;
}