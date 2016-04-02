#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.h"

/**
 * @brief	Gets the Camera that is currently active.		
 */
SDL_Rect Camera_GetActiveCamera();

/**
 * @brief	Get's the position of the active camera.
 *
 * @return	Returns the position as a Vec2d.
 */
Vec2d Camera_GetPosition();

/**
 * @brief	Get's the size of the active camera.
 *
 * @return	Returns the size as a Vec2d.
 */
Vec2d Camera_GetSize();

/**
 * @brief	Set's the size of the active camera.
 *
 * @param size	The size you want to set the camera to.
 */
void Camera_SetSize(Vec2d size);

/**
 * @brief	Set's the position of the active camera.
 *
 * @param size	The position you want to set the camera to.
 */
void Camera_SetPosition(Vec2d pos);

/**
 * @brief	Checks for collision between camera and entity.
 *
 * @param ent	The entity you're checking the collision against.
 */
int Camera_Intersect(Entity *ent);

#endif