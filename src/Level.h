#ifndef __LEVEL__H__
#define __LEVEL__H__

#include "Entity.h"
#include "Audio.h"
#include "Camera.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>
#include <SDL.h>
#include <glib.h>

#define SPLIT_VERTICAL 0 /**<Node split macro for splitting vertically */
#define SPLIT_HORIZONTAL 1 /**<Node split macro for splitting horizontally */

/**
 * @brief	Defines the structure for a Room data type.
 */
typedef struct Room_T
{
	int type;								/**<The RTYPE of the room */
	Vec2d size;								/**<The size of the room */
	Vec2d pos;								/**<The position of the room */
	SDL_Rect bounds;						/**<The room bounds */
	Sprite *image;							/**<The room background image */
	int numEnemy;							/**<Number of Enemies in the room */
	int val;								/**<Room value */
	struct Room_T *next;					/**<Linked List of rooms */
	struct Node_S *owner;
	Entity *north, *south, *east, *west;	/**<Door entities */

	void (*draw)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
}Room;

/**
 * @brief	Defines the structure for a Node data type.
 */
typedef struct Node_S
{
	char id;				/**<Node id */
	int inuse;				/**<Node inuse flag */
	Room *room;				/**<Room inside this node */

	int width;				/**<Node width */
	int height;				/**<Node height */
	int split;				/**<The split this node is doing */
	Vec2d pos;				/**<Position of the node */
	struct Node_S *parent;	/**<The parent of this node */
	union
	{
		Node_S *left;		/**<The left child node */
		Node_S *top;		/**<The top child node */
	};
	union
	{
		Node_S *right;		/**<The right child node */
		Node_S *bottom;		/**<The bottom child node */
	};

	void (*drawroom)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos); /**<Draw room function pointer */
}Node;

////////////////////////////////////////////////////////////
///////////////////LEVEL LOADER FUNCTIONS///////////////////
////////////////////////////////////////////////////////////
/**
 * @brief	Level load.
 *
 * @param file	a string for the file that is being used to load the level
 * @param level	an int that tells the Level_Loader which level to load.
 */
void Level_Load(Uint8 levelType);

////////////////////////////////////////////////////////////
/////////////////////NODE FUNCTIONS/////////////////////////
////////////////////////////////////////////////////////////
/**
 * @brief	Node new.
 *
 * @return	The Node being created.
 */
Node *Node_New(int x, int y, int width, int height, Node *parent);

/**
 * @brief	Node new.
 *
 * @param n	The node being subdivided
 * @param count The number of subdivisions
 */
void Node_RecursiveSubDivide(Node *n, int count);
/**
 * @brief	Node free.
 *
 * @param *n	The node being freed.
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
GList *NodeList_Get();
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

/**
 * @brief	Creates a new room.
 *
 * @param pos	The position of the room
 * @param file	a string for the file that is being used to load the room background
 * @param rtype	an int that tells which room type is being used.
 */
Room *Room_New(Vec2d pos, char *file, int rtype, Node *owner);

/**
 * @brief	Creates a new door entity.
 *
 * @param x	The x position.
 * @param y	The y position.
 *
 * @return	A door entity with door information.
 */
Entity *Door_New(int x, int y);

/**
 * @brief	Creates rooms using a BSP Tree recursively.
 *
 * @param b	The head node of the created tree.
 * @param file The file of the background for the room being created
 */
void Room_RecursiveCreateRoom(Node *n, char *file);

/**
 * @brief	Links rooms by creating Door entities.
 *
 * @param l	The left or top room
 * @param r	The right or bottom room
 * @param split The node split of the parent
 */
void Room_Link(Room *l, Room *r, int split);

/**
 * @brief	Door Entity Think.
 *
 * @param door	The door entity that is thinking.
 */
void Door_Think(Entity *door);

/**
 * @brief	Door Entity Touch.
 *
 * @param door	the door entity that is being touched.
 * @param other	the entity that is touching.
 */
void Door_Touch(Entity *door, Entity *other);

/**
 * @brief	Room draw for every room in a node.
 */
void Room_DrawAll();

////////////////////////////////////////////////////////////
/////////////////////HUB FUNCTIONS//////////////////////////
////////////////////////////////////////////////////////////
/**
 * @brief	Creates the static HUB world.
 *
 * @param file	The file that the HUB is being loaded from.
 */
void Hub_Create(char *file);

GList *RoomList_Get();
GList *LeafList_Get();
void Door_Placer();
#endif