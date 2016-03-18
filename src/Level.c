#include "Level.h"
#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>

Node *head;
Node *NList;
Room *r;
Room *roomList;
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
	r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, 250, 100);
	r->pos = pos;

	r->type = RTYPE_NORMAL;
	r->numEnemy = rand() % 9;
	r->image = sprite_Load("images/room.png",r->size.x,r->size.y);

	r->next = roomList;
	roomList = r;
	return r;
}

Entity *Door_New(int x, int y)
{
	Entity *door;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	door = Entity_New("images/hall.png", 10,10, gPos);

	if(door)
	{
		door->touch = &Door_Touch;

		door->type = OTHER;
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
		r->val = ++length;			
	}
}

void Room_Link(Room *l, Room *r, int split)
{
	if(split == SPLIT_HORIZONTAL) //if parent was split horizontally, 
								  // create doors in the south of the left room and north of the right room
	{
		l->south = Door_New(l->pos.x+l->size.x/2-5,l->pos.y+l->size.y-10);
		r->north = Door_New(r->pos.x+r->size.x/2-5, r->pos.y);

		l->south->target = r->north;
		r->north->target = l->south;
	}
	else if(split == SPLIT_VERTICAL) //if parent was split vertically, 
									 // create doors in the east of the left room and west of the right room
	{	
		l->east = Door_New(l->pos.x+l->size.x-10,l->pos.y+l->size.y/2-5);
		r->west = Door_New(r->pos.x,r->pos.y+r->size.y/2-5);

		l->east->target = r->west;
		r->west->target = l->east;
	}
}

/////////////////////////////////////////////////////////
//					TOUCH FUNCTIONS					   //
/////////////////////////////////////////////////////////

void Door_Touch(Entity *door, Entity *other)
{
	if(other == Entity_GetByID(0))
	{
		other->pos = door->target->pos;
	}
	//Camera_SetPosition(Entity_GetByID(0)->pos);
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
	head->width = 1600;
	head->height = 900;
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



















///////////////////////////ROOM_LINKER (1st attempt)\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/*SDL_Rect Room_Linker(Node *n, int split)
{
	SDL_Rect link = rect(0,0,0,0);
	if(split == SPLIT_VERTICAL)
	{
		if(n->split == SPLIT_VERTICAL)
		{	
			n->east = Door_New(n->pos.x+250-10,n->pos.y+100/2-5);
			n->west = Door_New(n->right->pos.x,n->right->pos.y+100/2-5);

			n->west->target = n->west;
			n->west->target = n->west;

			//slog("Room %i: Pos: %f, %f", n->left->room->val, n->left->room->east->pos.x, n->left->room->east->pos.y);
			//slog("Room %i: Pos: %f, %f", n->right->room->val, n->right->room->west->pos.x, n->right->room->west->pos.y);

			//link = rect(n->left->pos.x+250,n->left->pos.y+100/2-5,n->right->pos.x - n->left->pos.x, 10);
		}
		else if(n->split == SPLIT_HORIZONTAL)
		{
			n->top->south = Door_New(n->top->pos.x+120, n->top->pos.y+100-10);
			n->bottom->north = Door_New(n->bottom->pos.x+250/2-5, n->bottom->pos.y);

			n->top->south->target = n->bottom->north;
			n->bottom->north->target = n->top->south;

			//slog("Room %i: Pos: %f, %f", n->top->room->val, n->top->room->south->pos.x, n->top->room->pos.y);
			//slog("Room %i: Pos: %f, %f", n->bottom->room->val, n->bottom->room->north->pos.x, n->bottom->room->north->pos.y);
	
			//link = rect(n->top->pos.x+250/2-5, n->top->pos.y+100, 10, n->bottom->pos.y - (n->top->pos.y));
		}
	}
	else
	{
		if(n->split == SPLIT_HORIZONTAL)
		{
			n->south = Door_New(n->pos.x+120, n->pos.y+100-10);
			n->bottom->north = Door_New(n->bottom->pos.x+250/2-5, n->bottom->pos.y);

			n->south->target = n->bottom->north;
			n->bottom->north->target = n->south;

			//slog("Room %i: Pos: %f, %f", n->top->room->val, n->top->room->south->pos.x, n->top->room->pos.y);
			//slog("Room %i: Pos: %f, %f", n->bottom->room->val, n->bottom->room->north->pos.x, n->bottom->room->north->pos.y);

			//link = rect(n->top->pos.x+250/2-5, n->top->pos.y+100, 10, n->bottom->pos.y - (n->top->pos.y));
		}
		else if(n->split == SPLIT_VERTICAL)
		{	
			n->left->east = Door_New(n->left->pos.x+250-10,n->left->pos.y+100/2-5);
			n->right->west = Door_New(n->right->pos.x,n->right->pos.y+100/2-5);

			n->right->west->target = n->left->east;
			n->left->east->target = n->right->west;

			//slog("Room %i: Pos: %f, %f", n->left->room->val, n->left->room->east->pos.x, n->left->room->east->pos.y);
			//slog("Room %i: Pos: %f, %f", n->right->room->val, n->right->room->west->pos.x, n->right->room->west->pos.y);

			//link = rect(n->left->pos.x+250,n->left->pos.y+100/2-5,n->right->pos.x - n->left->pos.x, 10);
		}
		else
		{
			link = rect(0,0,0,0);
		}
		return link;
	}
	return rect(0,0,0,0);
}*/