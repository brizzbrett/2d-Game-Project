#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>
#include "Vector.h"
#include "Sprite.h"
#include <SDL.h>

#define RTYPE_START 1
#define RTYPE_NORMAL 2
#define RTYPE_BOSS 3

#define SPLIT_VERTICAL 0
#define SPLIT_HORIZONTAL 1

/**
 * @brief	Defines an alias representing the structure.
 */
typedef struct Room_T
{
	int type;
	Vec2d size;
	Vec2d pos;
	Sprite *image;
	int numEnemy;
	int val;
	struct Room_T *next;
}Room;

/**
 * @brief	Defines an alias representing the node s.
 */
typedef struct Node_S
{
	char *id;
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

	SDL_Rect link;

	void (*drawroom)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
	void (*drawhall)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos, SDL_Rect r);
}Node;

/**
 * @brief	Node new.
 *
 * @return	null if it fails, else a Node*.
 */
Node *Node_New();

/**
 * @brief	Room new.
 *
 * @param	type	The type.
 * @param	pos 	The position.
 *
 * @return	null if it fails, else a Room*.
 */
Room *Room_New(int type, Vec2d pos);

/**
 * @brief	Room create.
 *
 * @param [in,out]	n	If non-null, the Node to process.
 *
 * @return	null if it fails, else a Room*.
 */
Room *Room_Create(Node *n);

/**
 * @brief	Room link.
 *
 * @param [in,out]	left 	If non-null, the left.
 * @param [in,out]	right	If non-null, the right.
 *
 * @return	An int.
 */
int Room_Link(Node *left, Node *right);

/**
 * @brief	Room draw 2.
 */
void Room_Draw2();

/**
 * @brief	Room draw.
 *
 * @param [in,out]	sprite  	If non-null, the sprite.
 * @param [in,out]	renderer	If non-null, the renderer.
 * @param	pos					The position.
 */
void Room_Draw(Sprite *sprite, SDL_Renderer *renderer, Vec2d pos);

/**
 * @brief	Room draw all.
 */
void Room_DrawAll();

/**
 * @brief	Level load.
 */
void Level_Load();

/**
 * @brief	Node free.
 *
 * @param [in,out]	n	If non-null, the Node to process.
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

void LinkedRooms();
#endif