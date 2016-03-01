#include "Level.h"

Node *head;
FILE *fpt;

void Room_Draw(Node *n);

Node *Node_New()
{
	Node* n = (Node *)malloc(sizeof(Node));

	n->room = NULL;
	n->width = 0;
	n->height = 0;
	n->split = 0;
	vec2d_Set(n->pos,0,0);
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;

	return n;
}
Room *Room_New(int type, Vec2d pos)
{
	Room *r = (Room *)malloc(sizeof(Room));

	vec2d_Set(r->size, 20,11);
	r->pos = pos;
	if(type == RTYPE_START)
	{
		r->type = RTYPE_START;
		r->numEnemy = 0;
		return r;
	}
	else
	{
		r->type = RTYPE_NORMAL;
		r->numEnemy = rand() % 9;
		return r;
	}
}
void Room_Link(Node *left, Node *right)
{
	if (abs(right->pos.y-left->pos.y) > abs(right->pos.x-left->pos.x)) 
	{
		if ((right->pos.y-left->pos.y) > 0) 
		{
			fprintf(fpt, "door is down\n");
		} 
		else if ((right->pos.y-left->pos.y) < 0) 
		{
			fprintf(fpt, "door is up\n");
		}
	} 
	else
	{
		if ((right->pos.x-left->pos.x) > 0) 
		{
			fprintf(fpt, "door is right\n");
		} 
		else if ((right->pos.x-left->pos.x) < 0) 
		{
			fprintf(fpt, "door is left\n");
		}
	}
}

Room *Room_Create(Node *n)
{	
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
		if(n->parent == NULL)
		{
			return Room_New(RTYPE_START,n->pos);
		}
		else
		{
			return Room_New(RTYPE_NORMAL, n->pos);
		}
	}
	return NULL;
}

void subdivide(Node *n)
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

		if(size < 10)
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

		if(size < 10)
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
void recursiveSubdivide(Node *n, int count)
{
	if(count <= 0)
	{
		return;
	}
	n->room = Room_Create(n);
	Room_Draw(n);
	subdivide(n);
	if(n->left)
	{
		recursiveSubdivide(n->left, count-1);
	}
	if(n->right)
	{
		recursiveSubdivide(n->right, count-1);
	}
	if(n->left && n->right){
		if(n->left->room != NULL && n->right->room != NULL)
		{
			Room_Link(n->left, n->right);
		}
	}
}

void Room_Draw(Node *n)
{
	int openTPos = 0;
	int enemies = n->room->numEnemy;
	for(int i = 0; i <= n->room->size.x-1; i++)
	{
		fprintf(fpt,"1");
	}
	fprintf(fpt,"\n");
	for(int i = 0; i <= n->room->size.y-3; i++)
	{
		fprintf(fpt,"1");

		for(int i = 0; i <= n->room->size.x-3; i++)
		{
				openTPos++;
				if(enemies != 0 && (openTPos == 11 || openTPos == 14 || openTPos == 17 || openTPos == 28 || openTPos == 32 || openTPos == 36 || openTPos == 39 || openTPos == 43)){
					fprintf(fpt,"E");
					enemies--;
				}
				else
				{
					fprintf(fpt,"0");
				}
		}
		fprintf(fpt,"1\n");
	}
	for(int i = 0; i <= n->room->size.x-1; i++)
	{
		fprintf(fpt,"1");
	}
	fprintf(fpt,"\n");
	fprintf(fpt,"%i", n->room->type);
	fprintf(fpt,"\n");
	fprintf(fpt,"\n");
}

void Level_Load()
{
	head = Node_New();
	head->width = 100;
	head->height = 50;
	srand(time(NULL));

	fpt = fopen("src/testlevel.txt", "w");

	recursiveSubdivide(head, 5);
	fclose(fpt);
}