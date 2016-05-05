#include "Level.h"

#include "Player.h"
#include "Enemy_Glop.h"
#include "Enemy_Eye.h"
#include "Enemy_Spider.h"
#include "Boss_Nightmare.h"
#include "Items.h"

static GList *hubList;
static GList *roomList;

Uint32 roomnum = 1;
Uint32 length = 0;
int bedlvl = 1;
Entity *p;

Room *Room_New(Vec2d pos, char *file, int levelin)
{
	Room *r;

	r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, ROOM_WIDTH, ROOM_HEIGHT);
	r->pos = pos;
	r->bounds = rect(r->pos.x+100,r->pos.y+100,r->size.x-200, r->size.x-200);
	
	r->image = sprite_Load(file,r->size.x,r->size.y);	
	r->val = roomnum;
	r->doors = 0;
	r->type = RTYPE_NORMAL;

	r->nroom = NULL;
	r->sroom = NULL;
	r->eroom = NULL;
	r->wroom = NULL;

	r->north = NULL;
	r->south = NULL;
	r->west = NULL;
	r->east = NULL;

	r->numEnemy = rand() % 6+1;
	r->levelin = levelin;
	if(r->levelin != 0)
	{
		r->frame = rand() % 4;
	}
	else
	{
		r->frame = 0;
	}
	r->draw = &sprite_Draw;
	roomnum += 1;
	
	if(!p)
	{
		p = Player_Load(r->pos.x+468,r->pos.y+325);
		slog("Player created!");
		Camera_SetPosition(r->pos);
	}	
	if(levelin == 0)
	{
		hubList = g_list_append(hubList, r);
	}
	else
	{
		roomList = g_list_append(roomList, r);
	}
	return r;
}

Entity *Door_New(int x, int y, EntityType type, int levelin)
{
	Entity *door;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	door = Entity_New(type, gPos);

	if(door)
	{
		door->touch = &Door_Touch;
		door->think = &Door_Think;

		door->owner = Entity_GetByType(PLAYER);
		door->target = NULL;
		door->levelin = levelin;
		door->keys = 0;
		door->boss = NONE;
		return door;
	}
	return NULL;
}

GList *RoomList_Get()
{
	return roomList;
}
GList *HubList_Get()
{
	return hubList;
}
void WinPath()
{
	GList *it;
	GList *it2;
	Room *r;
	Room *r2;
	Room *start;
	Room *finish;

	int keys = 0;

	float distance;
	float prevDistance = 0;

	for(it = DeadEnds_Get(); it != NULL; it = it->next)
	{
		r = (Room *)(it->data);
		r->type = RTYPE_KEY;
		keys++;
		for(it2 = DeadEnds_Get(); it2 != NULL; it2 = it2->next)
		{
			r2 = (Room *)(it2->data);
			if(r == r2)continue;
			distance = vec2d_GetDistanceSquared(r->pos,r2->pos);
			if(distance > prevDistance)
			{
				start = r;
				finish = r2;
				prevDistance = distance;
			}	
		}
	}
	if(p && !p->set)
	{
		if(!start)
		{
			p->pos.x = r->pos.x+478;
			p->pos.y = r->pos.y+500;
			r->type = RTYPE_START;
		}
		else
		{
			p->pos.x = start->pos.x+478;
			p->pos.y = start->pos.y+500;
			start->type = RTYPE_START;
			finish->type = RTYPE_BOSS;
			finish->keys = keys - 2;
		}
		slog("\n\nSTART AND FINISH");
		slog("\nROOM %c-------------\n\tPOSITION-- X: %.00f, Y: %.00f, Doors: %i, Type: %i", start->val+65, start->pos.x, start->pos.y, start->doors, start->type);
		slog("\nROOM %c-------------\n\tPOSITION-- X: %.00f, Y: %.00f, Doors: %i, Type: %i", finish->val+65, finish->pos.x, finish->pos.y, finish->doors, finish->type);
		slog("DISTANCE: %.0f", sqrt(distance));
		Camera_SetPosition(start->pos);
		p->set = TRUE;
	}

}
void Room_Populate(int levelin)
{
	GList *g;
	Room *r;
	Vec2d bed;
	Vec2d key;
	int i = 0;
	Entity *boss;

	for(g = hubList; g != NULL; g = g->next)
	{
		Room *r = (Room *)(g->data);
		vec2d_Set(bed,r->pos.x+150,r->pos.y+400);
		vec2d_Set(key,r->pos.x+484,r->pos.y+335);
		if(r->type == RTYPE_HUBR)
		{	
			Item_Spawn(Bed_New(bed,bedlvl), levelin);
			bedlvl++;
		}
	}
	for (g = roomList; g != NULL; g = g->next)
	{
		Room *r = (Room *)(g->data);
		vec2d_Set(bed,r->pos.x+150,r->pos.y+400);
		vec2d_Set(key,r->pos.x+484,r->pos.y+335);
		
		if(levelin == 1)
		{
			if(r->type == RTYPE_START)
			{
				r->numEnemy = 0;
			}
			else if(r->type == RTYPE_BOSS)
			{
				r->numEnemy = 0;
				Item_Spawn(Bed_New(bed,bedlvl), levelin);
				if(r->north)
				{
					r->north->boss = BDOOR;
					r->north->keys = r->keys;
					r->north->target->frame = 1;
				}
				else if(r->south)
				{
					r->south->boss = BDOOR;
					r->south->keys = r->keys;
					r->south->target->frame = 1;
				}
				else if(r->east)
				{
					r->east->boss = BDOOR;
					r->east->keys = r->keys;
					r->east->target->frame = 1;
				}
				else if(r->west)
				{
					r->west->boss = BDOOR;
					r->west->keys = r->keys;
					r->west->target->frame = 1;
				}
				slog("Keys: %i", r->keys);
			}
			else if(r->type == RTYPE_KEY)
			{
				if(levelin == 1)
				{
					boss = Nightmare_Load(key.x,key.y,levelin);
				}
				i++;
			}
			else if(r->type == RTYPE_NORMAL)
			{
				Room_MobMaker(r, levelin, r->numEnemy);
			}
		}
	}
}
void Room_MobMaker(Room *r, int levelin, int numEnemy)
{	
	int i;

	Vec2d availP[6];

	Entity *spider[6];
	int spiderCount = 0;

	Entity *glop[6];
	int glopCount = 0;

	Entity *eye[6];
	int eyeCount = 0;

	int randomPlayer = rand() % 5;
	int randomEnemy = rand() % 3;

	vec2d_Set(availP[0], r->pos.x+150, r->pos.y+125);
	vec2d_Set(availP[1], r->pos.x+370, r->pos.y+325);
	vec2d_Set(availP[2], r->pos.x+750, r->pos.y+125);
	vec2d_Set(availP[3], r->pos.x+570, r->pos.y+325);
	vec2d_Set(availP[4], r->pos.x+150, r->pos.y+525);
	vec2d_Set(availP[5], r->pos.x+750, r->pos.y+525);

	for(i = 0; i < numEnemy; i++)
	{
		randomEnemy = rand() % 3;

		if(randomEnemy == 0)
		{
			spider[spiderCount] = Spider_Load(availP[i].x,availP[i].y, levelin);
			spiderCount++;
		}
		else if(randomEnemy == 1)
		{

			eye[eyeCount] = Eye_Load(availP[i].x,availP[i].y, levelin);
			eyeCount++;

		}
		else
		{
			glop[glopCount] = Glop_Load(availP[i].x,availP[i].y, levelin);
			glopCount++;
		}
	}
}

void Room_Link(Room *l, Room *r, int split, int levelin)
{
	if(split == SPLIT_HORIZONTAL) //if parent was split horizontally, 
								  // create doors in the south of the left room and north of the right room
	{
		l->south = Door_New(l->pos.x+l->size.x/2-45,l->pos.y+l->size.y-115, SDOOR, levelin);
		r->north = Door_New(r->pos.x+r->size.x/2-45, r->pos.y+35,NDOOR, levelin);

		if(!l->south && !r->north)
		{
			return;
		}
		l->south->target = r->north;
		r->north->target = l->south;
	}
	else if(split == SPLIT_VERTICAL) //if parent was split vertically, 
									 // create doors in the east of the left room and west of the right room
	{	
		l->east = Door_New(l->pos.x+l->size.x-120,l->pos.y+l->size.y/2-40, EDOOR, levelin);
		r->west = Door_New(r->pos.x+35,r->pos.y+r->size.y/2-50, WDOOR, levelin);

		if(!l->east && !r->west)
		{
			return;
		}
		l->east->target = r->west;
		r->west->target = l->east;
	}

	l->doors += 1;
	r->doors += 1;
}
void Room_Free(Room **r)
{
	
	if(!r)return;
	if(!*r)return;

	sprite_Free(&(*r)->image);
	(*r)->image = NULL;
	*r = NULL;
}
void Room_FreeByLevel(int level)
{
	Room *r; /**<alias for *ent*/
	GList *g;
	for (g = roomList; g != NULL; g = g->next)
	{
		r = (Room *)(g->data);
		Room_Free(&r);
	}
	//g_list_foreach(roomList, (GFunc)(g_free), NULL);
	g_list_free(roomList);
	roomList = NULL;
}

/////////////////////////////////////////////////////////
//				TOUCH/THINK/DRAW FUNCTIONS			   //
/////////////////////////////////////////////////////////

void Door_Think(Entity *door)
{
	GList *l;
	Room *r;
	if(SDL_GetTicks() >= door->nextThink)
	{
		door->touch = &Door_Touch;
	}
}

void Door_Touch(Entity *door, Entity *other)
{
	Vec2d position;
	if(other == Entity_GetByType(PLAYER))
	{
		door->target->touch = NULL;
		door->target->nextThink = SDL_GetTicks() + 1500;
		
		if(door->target->boss == BDOOR && other->keys != door->target->keys)
		{
			door->frame = 1;
		}
		else if(door->target->type == NDOOR)
		{
			door->frame = 0;
			vec2d_Set(position, Camera_GetPosition().x, door->target->pos.y-35);
			other->pos.y = door->target->pos.y + door->target->bounds.h+5;
			other->pos.x = door->target->pos.x;
			Camera_SetPosition(position);
			slog("Current room pos: %.00f, %.00f", position.x, position.y);
		}
		else if(door->target->type == SDOOR)
		{
			door->frame = 0;
			vec2d_Set(position, Camera_GetPosition().x, door->target->pos.y-ROOM_HEIGHT+115);
			other->pos.y = door->target->pos.y - other->sprite->frameSize.y-1;
			other->pos.x = door->target->pos.x;
			Camera_SetPosition(position);
			slog("Current room pos: %.00f, %.00f", position.x, position.y);
		}
		else if(door->target->type == WDOOR)
		{
			door->frame = 0;
			vec2d_Set(position, door->target->pos.x-35, Camera_GetPosition().y);
			other->pos.y = door->target->pos.y;
			other->pos.x = door->target->pos.x + door->target->bounds.w+5;
			Camera_SetPosition(position);
			slog("Current room pos: %.00f, %.00f", position.x, position.y);
		}
		else if(door->target->type == EDOOR)
		{
			door->frame = 0;
			vec2d_Set(position, door->target->pos.x-ROOM_WIDTH+120, Camera_GetPosition().y);
			other->pos.y = door->target->pos.y;
			other->pos.x = door->target->pos.x - other->sprite->frameSize.x-1;
			Camera_SetPosition(position);
			slog("Current room pos: %.00f, %.00f", position.x, position.y);
		}
	}
}

void Room_DrawAll()
{
	Uint32 i;
	GList *g;
	Room *r;
	for (g = HubList_Get(); g != NULL; g = g->next)
	{
		r = (Room *)(g->data);
		r->draw(r->image, r->frame, Graphics_GetActiveRenderer(), r->pos);
	}
	for (g = RoomList_Get(); g != NULL; g = g->next)
	{
		r = (Room *)(g->data);
		r->draw(r->image, r->frame, Graphics_GetActiveRenderer(), r->pos);
	}
}