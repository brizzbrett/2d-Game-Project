#include "Level.h"
#include "Graphics.h"
#include <string.h>

Node *head;
FILE *fpt;
Node *NList;
Node *leafList;
Uint32 length = 0;
Uint32 nodeMax = 100;
Uint32 numNodes = 0;

void Room_Draw2(Node *n);
SDL_Rect Room_Linker(Node *n);
void Hall_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos, SDL_Rect r);

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
		n->hall = Room_Linker(n);
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
	Room *r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, 250, 100);
	r->pos = pos;

	r->type = RTYPE_NORMAL;
	r->numEnemy = rand() % 9;
	r->image = sprite_Load("images/room.png",r->size.x,r->size.y);
	r->val = ++length;
	r->next = n->room;
	n->room = r;
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
	//Room *temp;
	if (n->left != NULL || n->right != NULL)
	{		
		if (n->left != NULL)
		{
			return Room_Create(n->left);
		}
		if (n->right != NULL)
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
	SDL_Rect hall;
	if(n->left->room && n->right->room){
		if(n->top->pos.x == n->bottom->pos.x)
		{
			hall = rect(n->top->room->pos.x+n->top->room->size.x/2-5, n->top->room->pos.y+n->top->room->size.y, 10, n->bottom->room->pos.y - (n->top->room->pos.y));//rect((n->left->pos.x+(n->left->width/2-2)),n->left->pos.y+n->left->height,4, 100);//n->right->pos.y-(n->left->pos.y+n->left->height));
		}
		else if(n->top->pos.y == n->bottom->pos.y)
		{
			hall = rect(n->left->room->pos.x+n->left->room->size.x,n->left->room->pos.y+n->left->room->size.y/2-5,n->right->room->pos.x - n->left->room->pos.x, 10);//n->right->pos.y-(n->left->pos.x+n->right->width),4);
		}
		else
		{
			hall = rect(0,0,0,0);
		}
		return hall;
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
		NList[i].drawhall(sprite_Load("images/hall.png",100,100), 0, Graphics_GetActiveRenderer(), NList[i].room->pos, NList[i].hall);
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

	fpt = fopen("src/testlevel.txt", "w");

	Node_RecursiveSubDivide(head, 5);
	fclose(fpt);
}

