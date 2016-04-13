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
static char *level = "def/levelcfg.txt";

void Level_Load(Uint8 levelType)
{
	GList *g;
	Room *r;
	Node *l;
	char *data;
	FILE *f;
	FILE *log;
	long len;
	Music *nightmare_music;
	cJSON *json, *root, *obj;

	if(!level)return;

	f = fopen(level, "rb");
	log = fopen("def/log.txt","w");
	if(!f)return;

	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);

	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);

	json = cJSON_Parse(data);
	if(!json)return;
	if(levelType == HUB)
	{
		Hub_Create(level);
	}
	else if(levelType == NIGHTMARE)
	{
		nightmare_music = Music_New("audio/nightmare.ogg",-1);
		Music_Player(nightmare_music);
		root = cJSON_GetObjectItem(json, "nightmare");
		if(!root)return;
		obj = cJSON_GetObjectItem(root, "level load");
		head = Node_New(0,0,cJSON_GetObjectItem(obj, "h width")->valueint,cJSON_GetObjectItem(obj, "h height")->valueint, NULL);

		srand(time(NULL));

		Node_RecursiveSubDivide(head, 4);
		Room_RecursiveCreateRoom(head, cJSON_GetObjectItem(obj, "room look")->valuestring);
		
		fprintf(log,"******NODES******");
		for (g = NodeList_Get(); g != NULL; g = g->next)
		{
			l = (Node *)(g->data);
			if(l->left || l->right)
				fprintf(log,"\nNODE %c-------------\n\tPOSITION-- X: %.00f, Y: %.00f \n\tSIZE-- W: %i, H:%i\n\tChildren: %c, %c", l->id, l->pos.x, l->pos.y, l->width, l->height, l->left->id, l->right->id);
			else
				fprintf(log,"\nNODE %c-------------\n\tPOSITION-- X: %.00f, Y: %.00f \n\tSIZE-- W: %i, H:%i", l->id, l->pos.x, l->pos.y, l->width, l->height);
		}
		fprintf(log,"\n******LEAFS******");
		for (g = LeafList_Get(); g != NULL; g = g->next)
		{
			l = (Node *)(g->data);
			fprintf(log,"\nNODE %c----PARENT %c----\n\tPOSITION-- X: %.00f, Y: %.00f\n\tRoom: %c--\n\t\tPOSITION-- X: %.00f, Y: %.00f\n\t\tSIZE-- W: %.00f, H:%.00f", l->id,l->parent->id,l->pos.x,l->pos.y, l->room->val+65,l->room->pos.x,l->room->pos.y, l->room->size.x, l->room->size.y);
		}
		fclose(log);
		//Door_Placer();
		atexit(Node_CloseSystem);
	}
}