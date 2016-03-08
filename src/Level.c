#include "Level.h"
#include "Graphics.h"
#include <string.h>
#include "simple_logger.h"

Node *head;
Node *NList;
Room *r;
Room *roomList;
Uint32 length = 0;
Uint32 nodeMax = 100;
Uint32 numNodes = 0;

void Room_Draw2(Node *n);
SDL_Rect Room_Linker(Node *n);
void Hall_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos, SDL_Rect r);
Entity *Door_Load(int x, int y);

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
		NList[i].id = "";
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
	n->room = Room_Create(n);
	if(n->left && n->right)
	{
		n->link = Room_Linker(n);
	}
}

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

/**
 * @brief	Room new.
 *
 * @param	type	The type.
 * @param	pos 	The position.
 *
 * @return	null if it fails, else a Room*.
 */
Room *Room_New(Node *n, int type, Vec2d pos)
{
	r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, 250, 100);
	r->pos = pos;

	r->type = RTYPE_NORMAL;
	r->numEnemy = rand() % 9;
	r->image = sprite_Load("images/room.png",r->size.x,r->size.y);

	r->north = NULL;
	r->south = NULL;
	r->east = NULL;
	r->west = NULL;

	r->val = ++length;
	r->next = roomList;
	slog("Value: %i ", length);
	roomList = r;
	return r;
}
/**
 * @brief	Room create.
 *
 * @param [in,out]	n	If non-null, the Node to process.
 *
 * @return	null if it fails, else a Room*.
 */
Room *Room_Create(Node *n)
{	
	if (n->left || n->right)
	{		
		if (n->left)
		{
			return Room_Create(n->left);
		}
		if (n->right)
		{
			return Room_Create(n->right);
		}
	}
	else
	{
		return Room_New(n,RTYPE_NORMAL, n->pos);
	}
	return NULL;
}

SDL_Rect Room_Linker(Node *n)
{
	SDL_Rect link;

	if(n->left->room && n->right->room){
		if(n->top->pos.x == n->bottom->pos.x)
		{
			n->top->room->south = Door_Load(n->top->room->pos.x+n->top->room->size.x/2-5, n->top->room->pos.y+n->top->room->size.y-10);
			n->bottom->room->north = Door_Load(n->bottom->room->pos.x+n->bottom->room->size.x/2-5, n->bottom->room->pos.y);

			n->top->room->south->target = n->bottom->room->north;
			n->bottom->room->north->target = n->top->room->south;	

			link = rect(n->top->room->pos.x+n->top->room->size.x/2-5, n->top->room->pos.y+n->top->room->size.y, 10, n->bottom->room->pos.y - (n->top->room->pos.y));
		}
		else if(n->top->pos.y == n->bottom->pos.y)
		{
			n->left->room->east = Door_Load(n->left->room->pos.x+n->left->room->size.x-10,n->left->room->pos.y+n->left->room->size.y/2-5);
			n->right->room->west = Door_Load(n->right->room->pos.x,n->right->room->pos.y+n->left->room->size.y/2-5);

			n->right->room->west->target = n->left->room->east;
			n->left->room->east->target = n->right->room->west;

			link = rect(n->left->room->pos.x+n->left->room->size.x,n->left->room->pos.y+n->left->room->size.y/2-5,n->right->room->pos.x - n->left->room->pos.x, 10);
		}
		else
		{
			link = rect(0,0,0,0);
		}
		return link;
	}
	return rect(0,0,0,0);
}
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

Entity *Door_Load(int x, int y)
{
	Entity *door;
	Vec2d gPos;
	vec2d_Set(gPos,x,y);

	door = Entity_New("images/hall.png", 10,10, gPos,Entity_GetByID(0));

	if(door)
	{
		door->think = &Door_Think;
		door->touch = &Door_Touch;

		door->type = OTHER;
		door->bounds = rect(0, 0, door->sprite->frameSize.x,door->sprite->frameSize.y);

		door->nextThink = 0;
		door->thinkRate = 100;

		door->owner = NULL;
		door->target = NULL;
		return door;
	}
	return NULL;
}
void Door_Think(Entity *door)
{
	if(SDL_GetTicks() >= door->nextThink)
	{
		door->nextThink = SDL_GetTicks() + door->thinkRate;
	}
}
void Door_Touch(Entity *door)
{
	Entity_GetByID(0)->pos = door->target->pos;
	//Camera_SetPosition(Entity_GetByID(0)->pos);
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
}

