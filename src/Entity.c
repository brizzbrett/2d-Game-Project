#include "Entity.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Camera.h"

static Entity *entList; /**<static global Entity List*/
static Uint32 entMax = 1000; /**<static unsigned 32-bit integer of maximum entities*/
//GList *g_entList;
Uint32 numEnt = 0; /**<unsigned 32-bit integer numEnt*/

Entity *Entity_New(EntityType type, Vec2d pos)
{
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	FILE *f;
	long len;
	char *data;
	cJSON *json,*root,*obj,*buf;
	Entity *ent = (Entity *)malloc(sizeof(Entity));
	
	f=fopen("def/entitycfg.txt","rb");
	if(!f)return NULL;
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);

	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);

	json = cJSON_Parse(data);
	if(!json)return NULL;
	root = cJSON_GetObjectItem(json,"entity");
	if(!root)return NULL;

	if(type == PLAYER)
		obj = cJSON_GetObjectItem(root, "player");
	else if(type == GLOP)
		obj = cJSON_GetObjectItem(root, "glop");
	else if(type == EYE)
		obj = cJSON_GetObjectItem(root, "eye");
	else if(type == SPIDER)
		obj = cJSON_GetObjectItem(root, "spider");
	else if(type == SHOT)
		obj = cJSON_GetObjectItem(root, "shot");
	else if(type == NIGHTBOSS)
		obj = cJSON_GetObjectItem(root, "nightboss");
	else if(type == PICKUP_HEART)
		obj = cJSON_GetObjectItem(root, "pickup heart");
	else if(type == PICKUP_TEMPHEART)
		obj = cJSON_GetObjectItem(root, "pickup tempheart");
	else if(type == BOULDER)
		obj = cJSON_GetObjectItem(root, "boulder");
	else if(type == BED)
		obj = cJSON_GetObjectItem(root, "bed");
	else if(type == KEY)
		obj = cJSON_GetObjectItem(root, "key");
	else if(type == SDOOR)
		obj = cJSON_GetObjectItem(root, "south door");
	else if(type == NDOOR)
		obj = cJSON_GetObjectItem(root, "north door");
	else if(type == EDOOR)
		obj = cJSON_GetObjectItem(root, "east door");
	else if(type == WDOOR)
		obj = cJSON_GetObjectItem(root, "west door");

	ent->type = type;
	buf = cJSON_GetObjectItem(obj, "imagefiles");

	ent->frame = cJSON_GetObjectItem(buf, "frame")->valueint;
	ent->pos = pos;
	ent->startPos = pos;
	ent->sprite = sprite_Load(cJSON_GetObjectItem(buf, "file")->valuestring,cJSON_GetObjectItem(buf, "fw")->valueint,cJSON_GetObjectItem(buf, "fh")->valueint);		

	buf = cJSON_GetObjectItem(obj, "bounds");

	ent->bounds = rect(
					cJSON_GetObjectItem(buf, "x")->valueint,
					cJSON_GetObjectItem(buf, "y")->valueint,
					cJSON_GetObjectItem(buf, "w")->valueint,
					cJSON_GetObjectItem(buf, "h")->valueint);

	buf = cJSON_GetObjectItem(obj, "info");

	ent->thinkRate = cJSON_GetObjectItem(buf, "think rate")->valueint;
	ent->nextThink = cJSON_GetObjectItem(buf, "next think")->valueint;
	ent->fireRate = cJSON_GetObjectItem(buf, "fire rate")->valueint;
	ent->nextFire = cJSON_GetObjectItem(buf, "next fire")->valueint;

	ent->strength = cJSON_GetObjectItem(buf, "strength")->valueint;
	ent->speed = cJSON_GetObjectItem(buf, "speed")->valueint;
	ent->health = cJSON_GetObjectItem(buf, "health")->valuedouble;
	ent->maxHealth = cJSON_GetObjectItem(buf, "max health")->valueint;
	

	for(i = 0; i < entMax; i++)
	{
		
		if(entList[i].inuse)
		{
			continue;
		}
		
		memset(&entList[i],0,sizeof(Entity));
		entList[i] = *ent;
		entList[i].inuse = 1;
		numEnt++;
		if(numEnt > entMax)
		{
			fprintf(stderr, "Maximum Sprites Reached.\n");
			exit(1);
		}
		if(entList[i].type == SHOT)
		{
			entList[i].drawbloom = &sprite_BloomDraw;
		}
		else
		{
			entList[i].draw = &sprite_Draw;
		}
		entList[i].update = NULL;
		entList[i].think = NULL;
		entList[i].touch = NULL;

		free(ent);
		//g_entList = g_list_append(g_entList, &entList[i]);
		return &entList[i];
	}
	return NULL;
}
int Entity_GetNum()
{
	return numEnt;
}

void Entity_Free(Entity **ent)
{

	if(!ent)return;
	if(!*ent)return;

	(*ent)->inuse = 0;

	sprite_Free(&(*ent)->sprite);
	(*ent)->sprite = NULL;
	*ent = NULL;

}
void Entity_FreeByLevel(int level)
{
	Entity *ent; /**<alias for *ent*/
	GList *g;
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < entMax; i++)
	{
		ent = &entList[i];
		if(ent->levelin == level)
		{
			Entity_Free(&ent);
		}
	}
}
void Entity_CloseSystem()
{
	Entity *ent; /**<alias for *ent*/
	GList *g;
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < entMax; i++)
	{
		ent = &entList[i];
        Entity_Free(&ent);
	}
	free(entList);

	entList = NULL;	
	numEnt = 0;
}

void Entity_InitSystem(Uint32 ent_Max)
{
	if(entMax == 0)
	{
		printf("Max Entity == 0\n");
		return;
	}
	entList = (Entity *)malloc(sizeof(Entity)*ent_Max);
	memset(entList,0,sizeof(Entity)*ent_Max);
	entMax = ent_Max;
	atexit(Entity_CloseSystem);
}

void Entity_DrawAll()
{
	Uint32 i;
	Entity *player = NULL;
	Vec3d color;
	vec3d_Set(color,255,0,0);
	for(i = 0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(!entList[i].draw)
		{
			continue;
		}
		if(entList[i].type == PLAYER)
		{
			player = &entList[i];
			continue;
		}
		if(entList[i].type == SHOT)
		{
			entList[i].drawbloom(entList[i].sprite, entList[i].frame, Graphics_GetActiveRenderer(), entList[i].pos, color, 20);
		}
		else
		{
			entList[i].draw(entList[i].sprite, entList[i].frame, Graphics_GetActiveRenderer(), entList[i].pos);
		}

		
	}
	if(player)
	{
		player->draw(player->sprite, player->frame, Graphics_GetActiveRenderer(), player->pos);
	}
}

void Entity_ThinkAll()
{
	Uint32 i;
	for(i = 0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(!entList[i].think)
		{
			continue;
		}
		entList[i].think(&entList[i]);

	}
}

void Entity_UpdateAll()
{
	Uint32 i;
	Uint32 pflag = 0;
	for(i= 0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(!entList[i].update)
		{
			continue;
		}
		entList[i].update(&entList[i]);		

		Entity_DrawAll();
		Entity_ThinkAll();
	}
}

void Entity_IntersectAll(Entity *a)
{
	int i;
	if(!a)return;

	for(i=0; i < entMax; i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(a == &entList[i])
		{
			continue;
		}	
		if(Entity_Intersect(a, &entList[i]))
		{			
			if(!a->inuse)
			{
				return;
			}

			if(a->touch)
			{
				a->touch(a, &entList[i]);
			}

			if(entList[i].touch)
			{
				entList[i].touch(&entList[i], a);
			}
		}
	}
	return;
}
int Entity_Intersect(Entity *a, Entity *b)
{
	SDL_Rect aB, bB;
	if ((!a) || (!b))
	{
		return 0;
	}
	aB = rect(a->pos.x+a->bounds.x,a->pos.y+a->bounds.y,a->bounds.w, a->bounds.h);
	bB = rect(b->pos.x+b->bounds.x,b->pos.y+b->bounds.y,b->bounds.w, b->bounds.h);
	if(rect_intersect(aB, bB))
		return 1;
	return 0;
}

Entity *Entity_GetByID(int id)
{
	Uint32 i;
	for(i=0; i < numEnt; i++)
	{
		if(entList[i].id == id)
		{
			return &entList[i];
		}
	}
	return NULL;
}
Entity *Entity_GetByType(EntityType type)
{
	Uint32 i;
	for(i=0; i < numEnt; i++)
	{
		if(entList[i].type == type)
		{
			return &entList[i];
		}
	}
	return NULL;
}
