#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>
#include "Vector.h"

#define RTYPE_START 1
#define RTYPE_NORMAL 2
#define RTYPE_BOSS 3

#define SPLIT_VERTICAL 0
#define SPLIT_HORIZONTAL 1

typedef struct
{
	int type;
	Vec2d size;
	Vec2d pos;
	int numEnemy;
}Room;

typedef struct Node_S
{
	Room *room;
	int width;
	int height;
	int split;
	Vec2d pos;
	Node_S *parent;
	union
	{
		Node_S *left;
		Node_S *top;
	};
	union
	{
		Node_S *right;
		Node_S *bottom;
	};
}Node;

Node *Node_New();
Room *Room_New(int type, Vec2d pos);
Room *Room_Create(Node *n);
void Room_Link(Node *left, Node *right);
void Room_Draw(Node *n);
void recursiveSubdivide(Node *n, int count);
void Level_Load();

#endif