#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Entity.h"
#include "Camera.h"
#include <SDL.h>

#define RTYPE_START 1
#define RTYPE_NORMAL 2
#define RTYPE_KEY 3
#define RTYPE_KEYDOOR 4
#define RTYPE_BOSS 5

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
	SDL_Rect bounds;
	Sprite *image;
	int numEnemy;
	int val;
	struct Room_T *next;
	Entity *north, *south, *east, *west;
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

	SDL_Rect link;

	Entity *north,*south,*east,*west;
	void (*drawroom)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
	void (*drawhall)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos, SDL_Rect r);
}Node;

/**
 * @brief	Level load.
 */
void Level_Load();
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
Room *Room_New(Node *n, Vec2d pos);
Entity *Door_New(int x, int y);
void Room_Link(Room *l, Room *r, int split);
void Room_IntersectAll(Entity *ent);
Room *Room_GetByID(int id);
void Door_Touch(Entity *door, Entity *other);

void Hall_Draw(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos, SDL_Rect r);
/**
 * @brief	Room draw all.
 */
void Room_DrawAll();

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

#endif