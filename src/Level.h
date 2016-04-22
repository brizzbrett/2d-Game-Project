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

	int levelin;

	int doors;
	int keys;

	int frame;

	struct Room_T *nroom, *sroom, *eroom, *wroom;
	Entity *north, *south, *east, *west;	/**<Door entities */

	void (*draw)(Sprite *sprite, int frame, SDL_Renderer *renderer, Vec2d pos);
	void (*touch)(struct Room_T *r, Entity *other);
}Room;

typedef struct Level_S
{
	char *data;
	FILE *f;
	long len;
	Music *bk_music;
}Level;

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

#define ROOM_WIDTH 1000
#define ROOM_HEIGHT 703

/**
 * @brief	Creates a new room.
 *
 * @param pos	The position of the room
 * @param file	a string for the file that is being used to load the room background
 * @param rtype	an int that tells which room type is being used.
 */
Room *Room_New(Vec2d pos, char *file, int levelin);
void Room_Free(Room **r);
void Room_FreeByLevel(int level);
void Room_MobMaker(Room *r, int levelin, int numEnemy);

/**
 * @brief	Creates a new door entity.
 *
 * @param x	The x position.
 * @param y	The y position.
 *
 * @return	A door entity with door information.
 */
Entity *Door_New(int x, int y, EntityType type, int levelin);

void Level_Maker(char *file, int levelin);
void Level_Closer(int level);

/**
 * @brief	Links rooms by creating Door entities.
 *
 * @param l	The left or top room
 * @param r	The right or bottom room
 * @param split The node split of the parent
 */
void Room_Link(Room *l, Room *r, int split, int levelin);

void Room_Touch(Room *r, Entity *other);

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
GList *DeadEnds_Get();
void Room_Populate(int levelin);
void WinPath();
#endif