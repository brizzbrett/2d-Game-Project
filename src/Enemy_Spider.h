#ifndef __ENEMY_SPIDER_H__
#define __ENEMY_SPIDER_H__

#include "Entity.h"
#include "Enemy_Fire.h"

typedef Entity Spider;

/**
 * @brief	Loads the spider at specified position.
 *
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 *
 * @return	null if it fails, else a Spider*.
 */

Spider *Spider_Load(int x, int y);

/**
 * @brief	Spider think.
 *
 * @param	*Spider		If non-null, the Spider.
 *
 */
void Spider_Think(Spider *Spider);

/**
 * @brief	Spider update.
 *
 * @param [in,out]	Spider	If non-null, the Spider.
 */
void Spider_Update(Spider *Spider);

/**
 * @brief	Spider touch.
 *
 * @param [in,out]	Spider	If non-null, the Spider.
 */
void Spider_Touch(Spider *Spider, Entity *other);

#endif