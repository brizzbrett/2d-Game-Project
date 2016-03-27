#include "Level.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>

Node *NList;

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

/////////////////////////////////////////////////////////
//					DRAW FUNCTIONS					   //
/////////////////////////////////////////////////////////

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
		if(!NList[i].room)
		{
			continue;
		}
		NList[i].drawroom(NList[i].room->image, 0, Graphics_GetActiveRenderer(), NList[i].room->pos);
	}
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

