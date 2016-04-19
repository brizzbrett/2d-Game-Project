#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "Entity.h"

/**
 * @brief	Item Entity Spawner.
 *
 * @return	The item entity being created, if not null.
 */
void Item_Spawn(Entity *item, int levelin);

/**
 * @brief	Item Entity Update.
 *
 * @param item	The item that is updating.
 */
void Item_Update(Entity *item);

/**
 * @brief	Pickup Entity Think.
 *
 * @param item	The item that is thinking.
 */
void Pickup_Think(Entity *item);

/**
 * @brief	Pickup Heart Entity Load.
 *
 * @param pos	The position.
 *
 * @return	The Heart entity being created, if not null.
 */
Entity *Pickup_Heart_New(Vec2d pos);

/**
 * @brief	Pickup TempHeart Entity Load.
 *
 * @param pos	The position.
 *
 * @return	The Temporary Heart entity being created, if not null.
 */
Entity *Pickup_TempHeart_New(Vec2d pos);

/**
 * @brief	Boulder Entity Load.
 *
 * @param pos	The position
 *
 * @return	The Boulder entity being created, if not null.
 */
Entity *Boulder_New(Vec2d pos);

/**
 * @brief	Bed Entity Load.
 *
 * @param pos	The position.
 * @return	The Bed entity being created, if not null.
 */
Entity *Bed_New(Vec2d pos, int bedlvl);

/**
 * @brief	Item Entity Touch.
 *
 * @param heart		The item that is being touched.
 * @param other		The item that is touching.
 */
void Pickup_Heart_Touch(Entity *heart, Entity *other);

/**
 * @brief	Item Entity Touch.
 *
 * @param tempheart	The item that is being touched.
 * @param other		The item that is touching.
 */
void Pickup_TempHeart_Touch(Entity *tempHeart, Entity *other);

/**
 * @brief	Item Entity Touch.
 *
 * @param boulder	The item that is being touched.
 * @param other		The item that is touching.
 */
void Boulder_Touch(Entity *boulder, Entity *other);

/**
 * @brief	Item Entity Touch.
 *
 * @param bed		The item that is being touched.
 * @param other		The item that is touching.
 */
void Bed_Touch(Entity *bed, Entity *other);
#endif