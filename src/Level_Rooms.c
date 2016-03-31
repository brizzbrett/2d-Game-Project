#include "Level.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>

#include "Player.h"
#include "Enemy_Glop.h"
#include "Enemy_Eye.h"
#include "Enemy_Spider.h"
#include "Pick_Ups.h"

Room *roomList;
Uint32 length = 0;

Entity *p;

Room *Room_New(Node *n, Vec2d pos)
{
	Room *r;

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

	int i;

	r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, ROOM_WIDTH, ROOM_HEIGHT);
	r->pos = pos;
	r->bounds = rect(r->pos.x+100,r->pos.y+100,r->size.x-200, r->size.x-200);
	r->type = RTYPE_NORMAL;
	r->numEnemy = rand() % 6+1;
	r->image = sprite_Load("images/room.png",r->size.x,r->size.y);

	r->val = length;
	length++;
	vec2d_Set(boulderPos,r->pos.x+randomBoulderX,r->pos.y+randomBoulderY);
	Pickup_Spawn(Boulder_New(boulderPos));
	if(!p && randomPlayer == 0)
	{
		p = Player_Load(r->pos.x+775,r->pos.y+600);
		slog("Player created!");
		Camera_SetPosition(r->pos);
		r->type = RTYPE_START;
		r->numEnemy = 0;
	}	
	
	vec2d_Set(availP[0], r->pos.x+150, r->pos.y+150);
	vec2d_Set(availP[1], r->pos.x+675, r->pos.y+450);
	vec2d_Set(availP[2], r->pos.x+1350, r->pos.y+150);
	vec2d_Set(availP[3], r->pos.x+875, r->pos.y+450);
	vec2d_Set(availP[4], r->pos.x+150, r->pos.y+750);
	vec2d_Set(availP[5], r->pos.x+1350, r->pos.y+750);

	for(i = 0; i < r->numEnemy; i++)
	{
		randomEnemy = rand() % 3;

		if(randomEnemy == 0)
		{
			spider[spiderCount] = Spider_Load(availP[i].x,availP[i].y);
			spiderCount++;
		}
		else if(randomEnemy == 1)
		{

			eye[eyeCount] = Eye_Load(availP[i].x,availP[i].y);
			eyeCount++;

		}
		else
		{
			glop[glopCount] = Glop_Load(availP[i].x,availP[i].y);
			glopCount++;
		}
	}

	return r;
}

Entity *Door_New(int x, int y, EntityType type)
{
	Entity *door;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	door = Entity_New(type, gPos);

	if(door)
	{
		door->touch = &Door_Touch;
		door->think = &Door_Think;
		door->type = type;

		door->owner = NULL;
		door->target = NULL;
		
		return door;
	}
	return NULL;
}

Room *Room_Get(Node *n)
{	
	Room *lRoom = NULL;
	Room *rRoom = NULL;

	if(n->room)
	{
		return n->room;
	}
	else
	{
		if (n->left)
		{
			lRoom = Room_Get(n->left);
		}
		if (n->right)
		{
			rRoom = Room_Get(n->right);
		}
		if(!lRoom && !rRoom)
		{
			return NULL;
		}		
		else if(!lRoom)
		{
			return rRoom;
		}
		else
		{
			return lRoom;
		}
	}
}

void Room_RecursiveCreateRoom(Node *n)
{
	if(n->left || n->right)
	{		
		if(n->left)
		{	
			Room_RecursiveCreateRoom(n->left);
		}
		if(n->right)
		{
			Room_RecursiveCreateRoom(n->right);
		}
		if(n->left && n->right) // if node has both left and right children, 
								// traverse these nodes until you get a room from both nodes and link them
		{
			Room_Link(Room_Get(n->left), Room_Get(n->right), n->split);
		}
	}
	else
	{
		n->room = Room_New(n, n->pos);		
	}
}

void Room_Link(Room *l, Room *r, int split)
{
	if(split == SPLIT_HORIZONTAL) //if parent was split horizontally, 
								  // create doors in the south of the left room and north of the right room
	{
		l->south = Door_New(l->pos.x+l->size.x/2-5,l->pos.y+l->size.y-70, SDOOR);
		r->north = Door_New(r->pos.x+r->size.x/2-5, r->pos.y+70,NDOOR);

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
		l->east = Door_New(l->pos.x+l->size.x-70,l->pos.y+l->size.y/2-5, EDOOR);
		r->west = Door_New(r->pos.x+70,r->pos.y+r->size.y/2-5, WDOOR);
		if(!l->east && !r->west)
		{
			return;
		}
		l->east->target = r->west;
		r->west->target = l->east;
	}
}

/////////////////////////////////////////////////////////
//					TOUCH FUNCTIONS					   //
/////////////////////////////////////////////////////////
// 
void Door_Think(Entity *door)
{
	if(SDL_GetTicks() >= door->nextThink && door->flag != 1)
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