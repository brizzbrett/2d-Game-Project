#include "Level.h"

GList *NList;
int id = 64;

Node *Node_New(int x, int y, int width, int height, Node *parent)
{
	Node *n = (Node *)malloc(sizeof(Node));
	memset(n,0,sizeof(Node));

	n->id = ++id;
	n->pos.x = x;
	n->pos.y = y;
	n->width = width;
	n->height = height;
	n->parent = parent;
	NList = g_list_append(NList, n);
	return n;
}

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

	if(n->split == SPLIT_VERTICAL)
	{
		size = n->width/2;

		if(size < 1600)
		{
			return;
		}
		a = Node_New(n->pos.x,n->pos.y,size,n->height,n);
		b = Node_New(n->pos.x+size,n->pos.y,n->width-size, n->height,n);

		n->left = a;
		n->right = b;
	}
	else
	{
		size = n->height/2;

		if(size < 900)
		{
			return;
		}
		a = Node_New(n->pos.x,n->pos.y,n->width,size,n);
		b = Node_New(n->pos.x,n->pos.y+size,n->width,n->height-size,n);

		n->top = a;
		n->bottom = b;
	}
	
}	
GList *NodeList_Get()
{
	return NList;
}
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

/*void Room_DrawAll()
{
	Uint32 i;

	for(i = 0; i < nodeMax; i++)
	{
		if(!n->inuse)
		{
			continue;
		}
		if(!n->drawroom)
		{
			continue;
		}
		if(!n->room)
		{
			continue;
		}
		n->drawroom(n->room->image, 0, Graphics_GetActiveRenderer(), n->room->pos);
	}
}*/

/////////////////////////////////////////////////////////
//					MEMORY MANAGEMENT				   //
/////////////////////////////////////////////////////////

void Node_Free(Node **n)
{
	//Entity *self; /**<alias for *ent*/
	if(!(*n))return;
	if(!n)return;
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

void Node_CloseSystem()
{
	GList *g;
	Node *n; /**<alias for *ent*/
	Uint32 i; /**<unsigned integer used for incrementing a for loop*/
	for(g = NList; g != NULL; g = g->next)
	{
		n = (Node *)(g->data);
        Node_Free(&n);
	}

	free(g);
	NList = NULL;
}
/*
void Node_InitSystem()
{
	if(nodeMax == 0)
	{
		printf("Max Nodes == 0\n");
		return;
	}
	NList = (Node *)malloc(sizeof(Node)*nodeMax);
	memset(NList,0,sizeof(Node)*nodeMax);
}*/

