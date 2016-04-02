#ifndef __LEVEL__H__
#define __LEVEL__H__

#include "Entity.h"
#include "Camera.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>
#include <SDL.h>

#define SPLIT_VERTICAL 0
#define SPLIT_HORIZONTAL 1

typedef struct Room_T
{
	int type;
	Vec2d size;
	Vec2d pos;
	SDL_Rect bounds;
	Sprite *image;
	int numEnemy;
	int val;
	struct Room_T *next;
	Entity *north, *south, *east, *west;

	void (*draw)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
}Room;

/**
 * @brief	Defines an alias representing the node s.
 */
typedef struct Node_S
{
	char id;
	int inuse;
	Room *room;

	int width;
	int height;
	int split;
	Vec2d pos;
	struct Node_S *parent;
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

	void (*drawroom)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
}Node;

/**
 * @brief	Level load.
 */
void Level_Load(char *file);
/**
 * @brief	Node new.
 *
 * @return	null if it fails, else a Node*.
 */
Node *Node_New();

void Node_RecursiveSubDivide(Node *n, int count);
/**
 * @brief	Node free.
 *
 * @param	*n	If non-null, the Node to process.
 */
void Node_Free(Node **n);
/**
 * @brief	Node close system.
 */
void Node_CloseSystem();
/**
 * @brief	Node initialise system.
 */
void Node_InitSystem();

////////////////////////////////////////////////////////////
/////////////////////ROOM FUNCTIONS/////////////////////////
////////////////////////////////////////////////////////////

#define RTYPE_START 1
#define RTYPE_NORMAL 2
#define RTYPE_KEY 3
#define RTYPE_KEYDOOR 4
#define RTYPE_BOSS 5
#define RTYPE_HUBR 6
#define RTYPE_HUBH 7
#define RTYPE_HUBM 8

#define ROOM_WIDTH 1600
#define ROOM_HEIGHT 900


Room *Room_New(Vec2d pos, char *file, int rtype);
Entity *Door_New(int x, int y);
void Room_RecursiveCreateRoom(Node *n, char *file);
void Room_Link(Room *l, Room *r, int split);
void Room_IntersectAll(Entity *ent);
Room *Room_GetByID(int id);
void Door_Think(Entity *door);
void Door_Touch(Entity *door, Entity *other);
void Room_DrawAll();

void Hub_Create(char *file);

#endif