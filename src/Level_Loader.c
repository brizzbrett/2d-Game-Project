#include "Level.h"
#include <cmath>
#include <time.h>
#include <random>

#define HUB 0
#define NIGHTMARE 1
#define COWBOY 2
#define FUTURE 3
#define MEDIEVAL 4

Node *head;

void Level_Load(char *file, Uint8 level)
{
	char *data;
	FILE *f;
	long len;
	cJSON *json, *root, *obj;

	if(!file)return;

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
	if(level == HUB)
	{
		Hub_Create(file);
	}
	else if(level == NIGHTMARE)
	{
		
		root = cJSON_GetObjectItem(json, "nightmare");
		if(!root)return;
		obj = cJSON_GetObjectItem(root, "level load");
		head = Node_New();
		head->width = cJSON_GetObjectItem(obj, "head width")->valueint;
		head->height = cJSON_GetObjectItem(obj, "head height")->valueint;

		srand(time(NULL));

		Node_RecursiveSubDivide(head, cJSON_GetObjectItem(obj, "divisions")->valueint);
		Room_RecursiveCreateRoom(head, cJSON_GetObjectItem(obj, "room look")->valuestring);
	}
}