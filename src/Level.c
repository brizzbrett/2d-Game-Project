#include "Level.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>

#include "Enemy_Glop.h"
#include "Enemy_Eye.h"
#include "Enemy_Spider.h"
#include "Player.h"
#include "Pick_Ups.h"

#define ROOM_WIDTH 1600
#define ROOM_HEIGHT 900

Node *head;
Node *NList;
Room *roomList;
Player *p;
Uint32 length = 0;
int id = 96;
Uint32 nodeMax = 100;
Uint32 numNodes = 0;

/**
 * @brief	Node new.
 *
 * @return	null if it fails, else a Node*.
 */
Node *Node_New()
{
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < nodeMax; i++)
	{
		if(NList[i].inuse)
		{
			continue;
		}

		memset(&NList[i],0,sizeof(Node));
		NList[i].id = 'a';
		NList[i].inuse = 1;

		numNodes++;

		NList[i].room = NULL;
		NList[i].width = 0;
		NList[i].height = 0;
		NList[i].split = 0;
		vec2d_Set(NList[i].pos,0,0);
		NList[i].parent = NULL;
		NList[i].left = NULL;
		NList[i].right = NULL;
		NList[i].drawroom = &sprite_Draw;
		NList[i].drawhall = &Hall_Draw;

		return &NList[i];
	}
	return NULL;
}

/**
 * @brief	Node sub divide.
 *
 * @param [in,out]	n	If non-null, the Node to process.
 */
void Node_SubDivide(Node *n)
{
	int size = 0;
	Node *a;
	Node *b;

	if(n == NULL)
	{
		return;
	}
	n->split = rand() % 2;
	n->id = ++id;
	a = Node_New();
	b = Node_New();
	a->parent = n;
	b->parent = n;
	a->pos = n->pos;
	if(n->split == SPLIT_VERTICAL)
	{
		size = n->width/2;

		if(size < 250)
		{
			return;
		}

		a->width = size;
		b->width = n->width-size;
		a->height = n->height;
		b->height = n->height;
		b->pos.x = n->pos.x+size;
		b->pos.y = n->pos.y;

		n->left = a;
		n->right = b;
	}
	else
	{
		size = n->height/2;

		if(size < 100)
		{
			return;
		}

		a->height = size;
		b->height = n->height-size;
		a->width = n->width;
		b->width = n->width;
		b->pos.x = n->pos.x;
		b->pos.y = n->pos.y+size;

		n->top = a;
		n->bottom = b;
	}
	
}	

/**
 * @brief	Node recursive sub divide.
 *
 * @param [in,out]	n	If non-null, the Node to process.
 * @param	count	 	Number of.
 */
void Node_RecursiveSubDivide(Node *n, int count)
{	

	if(count <= 0)
	{
		return;
	}

	Node_SubDivide(n);
	if(n->left)
	{
		Node_RecursiveSubDivide(n->left, count-1);
	}
	if(n->right)
	{
		Node_RecursiveSubDivide(n->right, count-1);
	}

}


/**
 * @brief	Room new.
 *
 * @param	type	The type.
 * @param	pos 	The position.
 *
 * @return	null if it fails, else a Room*.
 */
Room *Room_New(Node *n, Vec2d pos)
{
	Spider *spider[9];
	int spiderCount = 0;
	Glop *glop[9];
	int glopCount = 0;
	Eye *eye[9];
	int eyeCount = 0;
	int randomEnemy = rand() % 3;
	int randomPlayer = rand() % 5;
	Vec2d boulderPos;
	int randomBoulderX = 200;
	int randomBoulderY = 450;
	int i;

	Room *r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, ROOM_WIDTH, ROOM_HEIGHT);
	r->pos = pos;
	r->bounds = rect(r->pos.x+100,r->pos.y+100,r->size.x-200, r->size.x-200);
	r->type = RTYPE_NORMAL;
	r->numEnemy = rand() % 8;
	r->image = sprite_Load("images/room.png",r->size.x,r->size.y);

	r->val = length;
	length++;
	r->next = roomList;
	roomList = r;	
	vec2d_Set(boulderPos,r->pos.x+randomBoulderX,r->pos.y+randomBoulderY);
	Pickup_Spawn(Boulder_New(boulderPos));
	if(!p && randomPlayer == 0)
	{
		p = Player_Load(r->pos.x+775,r->pos.y+600);
		Camera_SetPosition(r->pos);
	}
	for(i = 0; i < r->numEnemy; i++)
	{
		randomEnemy = rand() % 3;

		if(randomEnemy == 0)
		{
			spider[spiderCount] = Spider_Load(r->pos.x*((i+1))+1300,r->pos.y+150);
			spiderCount++;
		}
		else if(randomEnemy == 1)
		{

			eye[eyeCount] = Eye_Load((r->pos.x*(i+1))+750,r->pos.y+450);
			eyeCount++;

		}
		else
		{
			glop[glopCount] = Glop_Load((r->pos.x*(i+1))+1300,r->pos.y+700);
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

	door = Entity_New("images/hall.png", 10,10, gPos);

	if(door)
	{
		door->touch = &Door_Touch;
		door->type = type;
		door->bounds = rect(0, 0, door->sprite->frameSize.x,door->sprite->frameSize.y);

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

		l->south->target = r->north;
		r->north->target = l->south;
	}
	else if(split == SPLIT_VERTICAL) //if parent was split vertically, 
									 // create doors in the east of the left room and west of the right room
	{	
		l->east = Door_New(l->pos.x+l->size.x-70,l->pos.y+l->size.y/2-5, EDOOR);
		r->west = Door_New(r->pos.x+70,r->pos.y+r->size.y/2-5, WDOOR);

		l->east->target = r->west;
		r->west->target = l->east;
	}
}
int Room_Intersect(Room *room, Entity *ent)
{
	SDL_Rect aB, bB;
	room = Room_Get(head);
	if(!ent)
	{
		return 0;
	}
	bB = rect(ent->pos.x,ent->pos.y,ent->bounds.w, ent->bounds.h);
	if(rect_intersect(room->bounds, bB))
		return 1;
	return 0;
}
void Room_IntersectAll(Entity *ent)
{
	int i;
	if(!ent)return;

	for(i=0; i < Entity_GetNum(); i++)
	{
		if(!ent->inuse)
		{
			continue;
		}
		if(Room_Intersect(Room_Get(head), ent))
		{
			
			//vec2d_Set(ent->vel,0,0);
		}
	}
	return;
}
/////////////////////////////////////////////////////////
//					TOUCH FUNCTIONS					   //
/////////////////////////////////////////////////////////

void Door_Touch(Entity *door, Entity *other)
{
	Vec2d position;
	if(other == Entity_GetByType(PLAYER))
	{
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

/////////////////////////////////////////////////////////
//					DRAW FUNCTIONS					   //
/////////////////////////////////////////////////////////

void Hall_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos, SDL_Rect r)
{
	SDL_Rect src,dest;

	src.x = frame%sprite->fpl * sprite->frameSize.x;
	src.y = frame/sprite->fpl * sprite->frameSize.y;
	src.w = sprite->frameSize.x;
	src.h = sprite->frameSize.y;

	dest = r;

	SDL_RenderCopy(renderer,sprite->image, &src, &dest);
}

/**
 * @brief	Room draw all.
 */
void Room_DrawAll()
{
	Uint32 i;
	for(i = 0; i < nodeMax; i++)
	{
		if(!NList[i].inuse)
		{
			continue;
		}
		if(!NList[i].drawroom)
		{
			continue;
		}
		if(!NList[i].drawhall)
		{
			continue;
		}
		if(!NList[i].room)
		{
			continue;
		}
		NList[i].drawroom(NList[i].room->image, 0, Graphics_GetActiveRenderer(), NList[i].room->pos);
		NList[i].drawhall(sprite_Load("images/hall.png",100,100), 0, Graphics_GetActiveRenderer(), NList[i].room->pos, NList[i].link);
	}
}

/**
 * @brief	Level load.
 */
void Level_Load()
{
	head = Node_New();
	head->width = 51200;
	head->height = 28800;
	srand(time(NULL));

	Node_RecursiveSubDivide(head, 5);
	Room_RecursiveCreateRoom(head);

}

/////////////////////////////////////////////////////////
//					MEMORY MANAGEMENT				   //
/////////////////////////////////////////////////////////
/**
 * @brief	Node free.
 *
 * @param [in,out]	n	If non-null, the Node to process.
 */
void Node_Free(Node **n)
{
	//Entity *self; /**<alias for *ent*/

	if(!n)return;
	if(!*n)return;
	if((*n)->room)
	{
		if((*n)->room->image)
		{
			sprite_Free(&(*n)->room->image);
			(*n)->room->image = NULL;
		}
		(*n)->room = NULL;
	}
	*n = NULL;
	n = NULL;

}

/**
 * @brief	Node close system.
 */
void Node_CloseSystem()
{
	Node *n; /**<alias for *ent*/
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(i = 0; i < nodeMax; i++)
	{
		n = &NList[i];
        Node_Free(&n);
	}
	free(NList);
	NList = NULL;	
	numNodes = 0;
}

/**
 * @brief	Node initialise system.
 */
void Node_InitSystem()
{
	if(nodeMax == 0)
	{
		printf("Max Nodes == 0\n");
		return;
	}
	NList = (Node *)malloc(sizeof(Node)*nodeMax);
	memset(NList,0,sizeof(Node)*nodeMax);
	atexit(Node_CloseSystem);
}

