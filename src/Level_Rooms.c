#include "Level.h"

#include "Player.h"
#include "Enemy_Glop.h"
#include "Enemy_Eye.h"
#include "Enemy_Spider.h"
#include "Items.h"

static GList *roomList;
int maxRooms = 15;

Uint32 roomnum = 1;
Uint32 length = 0;
int bedlvl = 1;
Entity *p;

Room *Room_New(Vec2d pos, char *file, int rtype, int levelin)
{
	Room *r;

	r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, ROOM_WIDTH, ROOM_HEIGHT);
	r->pos = pos;
	r->bounds = rect(r->pos.x+100,r->pos.y+100,r->size.x-200, r->size.x-200);
	
	r->image = sprite_Load(file,r->size.x,r->size.y);	
	r->val = roomnum;
	r->type = rtype;

	r->nroom = NULL;
	r->sroom = NULL;
	r->eroom = NULL;
	r->wroom = NULL;

	r->north = NULL;
	r->south = NULL;
	r->west = NULL;
	r->east = NULL;

	r->draw = &sprite_Draw;
	roomnum += 1;
	r->levelin = levelin;

	if(!p)
	{
		p = Player_Load(r->pos.x+775,r->pos.y+600);
		slog("Player created!");
		Camera_SetPosition(r->pos);
	}	

	Room_MobMaker(r, r->type, levelin);
	roomList = g_list_append(roomList, r);
	return r;
}
void Room_MobMaker(Room *r, int rtype, int levelin)
{	
	int i;

	Vec2d availP[6];

	Entity *spider[6];
	int spiderCount = 0;

	Entity *glop[6];
	int glopCount = 0;

	Entity *eye[6];
	int eyeCount = 0;

	int randomEnemy = rand() % 3;
	int randomPlayer = rand() % 5;

	Vec2d boulderPos;
	int randomBoulderX = 200;
	int randomBoulderY = 450;

	vec2d_Set(boulderPos,r->pos.x+randomBoulderX,r->pos.y+randomBoulderY);

	if(rtype == RTYPE_HUBR)
	{
		Item_Spawn(Bed_New(boulderPos,bedlvl), levelin);
		bedlvl++;
	}	
	if(rtype != RTYPE_HUBR && rtype != RTYPE_HUBH && rtype != RTYPE_HUBM)
	{
		r->type = RTYPE_NORMAL;
		r->numEnemy = rand() % 6+1;
		r->val = length;
		length++;
		vec2d_Set(boulderPos,r->pos.x+randomBoulderX,r->pos.y+randomBoulderY);
		
		if(p && randomPlayer == 0 && !p->set)
		{
			p->pos.x = r->pos.x+775;
			p->pos.y = r->pos.y+600;

			Camera_SetPosition(r->pos);
			r->type = RTYPE_START;
			r->numEnemy = 0;
			p->set = TRUE;
			Item_Spawn(Bed_New(boulderPos,bedlvl), levelin);
			bedlvl++;
		}	
		else
		{
			Item_Spawn(Boulder_New(boulderPos), levelin);
		}
		vec2d_Set(availP[0], r->pos.x+150, r->pos.y+150);
		vec2d_Set(availP[1], r->pos.x+675, r->pos.y+450);
		vec2d_Set(availP[2], r->pos.x+1350, r->pos.y+150);
		vec2d_Set(availP[3], r->pos.x+875, r->pos.y+450);
		vec2d_Set(availP[4], r->pos.x+150, r->pos.y+650);
		vec2d_Set(availP[5], r->pos.x+1250, r->pos.y+650);

		for(i = 0; i < r->numEnemy; i++)
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

		door->owner = NULL;
		door->target = NULL;
		
		door->levelin = levelin;
		return door;
	}
	return NULL;
}

GList *RoomList_Get()
{
	return roomList;
}

void Level_Maker(char *file, int levelin)
{
	Room *currRoom;
	Room *newRoom;
	int randPath;
	int i;
	Vec2d pos;
	vec2d_Set(pos,25600,14400);
	currRoom = Room_New(pos, file, RTYPE_START, levelin);

	for(i = 0; i < maxRooms; i++)
	{
		randPath = rand() % 4;

		if(randPath == 0)
		{
			if(currRoom->nroom)
			{
				currRoom = currRoom->nroom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x,currRoom->pos.y-ROOM_HEIGHT-100);
				newRoom = Room_New(pos, file, RTYPE_NORMAL, levelin);
				currRoom->nroom = newRoom;
				newRoom->sroom = currRoom;
				Room_Link(currRoom,newRoom, SPLIT_HORIZONTAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
		else if(randPath == 1)
		{
			if(currRoom->sroom)
			{
				currRoom = currRoom->sroom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x,currRoom->pos.y+ROOM_HEIGHT+100);
				newRoom = Room_New(pos, file, RTYPE_NORMAL, levelin);
				currRoom->sroom = newRoom;
				newRoom->nroom = currRoom;
				Room_Link(newRoom,currRoom, SPLIT_HORIZONTAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
		else if(randPath == 2)
		{
			if(currRoom->wroom)
			{
				currRoom = currRoom->wroom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x-ROOM_WIDTH-100,currRoom->pos.y);
				newRoom = Room_New(pos, file, RTYPE_NORMAL, levelin);
				currRoom->wroom = newRoom;
				newRoom->eroom = currRoom;
				Room_Link(newRoom,currRoom, SPLIT_VERTICAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
		else
		{
			if(currRoom->eroom)
			{
				currRoom = currRoom->eroom;
				i--;
			}
			else
			{
				vec2d_Set(pos,currRoom->pos.x+ROOM_WIDTH+100,currRoom->pos.y);
				newRoom = Room_New(pos, file, RTYPE_NORMAL, levelin);
				currRoom->eroom = newRoom;
				newRoom->wroom = currRoom;
				Room_Link(currRoom,newRoom, SPLIT_VERTICAL, levelin);
				currRoom = newRoom;
				newRoom = NULL;
			}
		}
	}

}
void Room_Link(Room *l, Room *r, int split, int levelin)
{
	if(split == SPLIT_HORIZONTAL) //if parent was split horizontally, 
								  // create doors in the south of the left room and north of the right room
	{
		l->south = Door_New(l->pos.x+l->size.x/2-5,l->pos.y+l->size.y-70, SDOOR, levelin);
		r->north = Door_New(r->pos.x+r->size.x/2-5, r->pos.y+70,NDOOR, levelin);

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
		l->east = Door_New(l->pos.x+l->size.x-70,l->pos.y+l->size.y/2-5, EDOOR, levelin);
		r->west = Door_New(r->pos.x+70,r->pos.y+r->size.y/2-5, WDOOR, levelin);
		if(!l->east && !r->west)
		{
			return;
		}
		l->east->target = r->west;
		r->west->target = l->east;
	}
}
void Room_Free(Room **r)
{
	
	if(!r)return;
	if(!*r)return;

	sprite_Free(&(*r)->image);
	//(*r)->image = NULL;
	*r = NULL;
}
void Room_FreeByLevel(int level)
{
	Room *r; /**<alias for *ent*/
	GList *g;

	for (g = RoomList_Get(); g != NULL; g = g->next)
	{
		r = (Room *)(g->data);
        if(r->levelin == level)
		{
			Room_Free(&r);
		}
	}
}

/////////////////////////////////////////////////////////
//					TOUCH/THINK FUNCTIONS			   //
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
		if(door->target->type == NDOOR)
		{
			vec2d_Set(position, Camera_GetPosition().x, door->target->pos.y-70);
			other->pos.y = door->target->pos.y +11;
			other->pos.x = door->target->pos.x - other->sprite->frameSize.x/2+5;
			Camera_SetPosition(position);
		}
		else if(door->target->type == SDOOR)
		{
			vec2d_Set(position, Camera_GetPosition().x, door->target->pos.y-ROOM_HEIGHT+70);
			other->pos.y = door->target->pos.y - other->sprite->frameSize.y-1;
			other->pos.x = door->target->pos.x - other->sprite->frameSize.x/2+5;
			Camera_SetPosition(position);
		}
		else if(door->target->type == WDOOR)
		{
			vec2d_Set(position, door->target->pos.x-70, Camera_GetPosition().y);
			other->pos.y = door->target->pos.y - other->sprite->frameSize.y/2+5;
			other->pos.x = door->target->pos.x + 11;
			Camera_SetPosition(position);
		}
		else if(door->target->type == EDOOR)
		{
			vec2d_Set(position, door->target->pos.x-ROOM_WIDTH+70, Camera_GetPosition().y);
			other->pos.y = door->target->pos.y - other->sprite->frameSize.y/2+5;
			other->pos.x = door->target->pos.x - other->sprite->frameSize.x-1;
			Camera_SetPosition(position);
		}
	}
}

void Room_DrawAll()
{
	Uint32 i;
	GList *g;
	Room *r;
	for (g = RoomList_Get(); g != NULL; g = g->next)
	{
		r = (Room *)(g->data);
		r->draw(r->image, 0, Graphics_GetActiveRenderer(), r->pos);
	}
}