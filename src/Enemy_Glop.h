#ifndef __ENEMY_GLOP_H__
#define __ENEMY_GLOP_H__

#include "Entity.h"

typedef Entity Glop;

/**
 * @brief	Glop load.
 *
 * @return	null if it fails, else a Glop*.
 */

Glop *Glop_Load(int x, int y);

/**
 * @brief	Glop think.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */

void Glop_Think(Glop *glop);

/**
 * @brief	Glop update.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */

void Glop_Update(Glop *glop);

/**
 * @brief	Glop touch.
 *
 * @param [in,out]	glop	If non-null, the glop.
 */

void Glop_Touch(Glop *glop);

#endif