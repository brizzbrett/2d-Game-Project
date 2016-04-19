#include "Level.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>

#include "Player.h"

void Hub_Create(char *file)
{
	Room *r[11];
	char *data;
	FILE *f;
	long len;
	Vec2d tVec;
	bool door[12];
	cJSON *json, *root, *obj, *room, *buf;

	f = fopen(file, "rb");

	if(!f)return;

	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);

	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);

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
	r[0] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBR, NULL);
	room = cJSON_GetObjectItem(obj, "hall1");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[1] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBH, NULL);
	room = cJSON_GetObjectItem(obj, "hall2");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[2] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBH, NULL);
	room = cJSON_GetObjectItem(obj, "hall3");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[3] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBH, NULL);
	room = cJSON_GetObjectItem(obj, "hall4");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[4] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBH, NULL);
	room = cJSON_GetObjectItem(obj, "cowboy room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[5] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBR, NULL);
	room = cJSON_GetObjectItem(obj, "future room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[6] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBR, NULL);
	room = cJSON_GetObjectItem(obj, "medieval room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[7] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBR, NULL);
	room = cJSON_GetObjectItem(obj, "final room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[8] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBR, NULL);
	room = cJSON_GetObjectItem(obj, "bath room");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[9] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBM, NULL);
	room = cJSON_GetObjectItem(obj, "kitchen");
	vec2d_Set(tVec, 
			  cJSON_GetObjectItem(room, "posx")->valueint, 
			  cJSON_GetObjectItem(room, "posy")->valueint
	);
	r[10] = Room_New(tVec, cJSON_GetObjectItem(room, "room look")->valuestring, RTYPE_HUBM, NULL);
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