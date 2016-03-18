#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <SDL.h>
#include "Entity.h"

SDL_Rect Camera_GetActiveCamera();
Vec2d Camera_GetPosition();
Vec2d Camera_GetSize();
void Camera_SetSize(Vec2d size);
void Camera_SetPosition(Vec2d pos);
int Camera_Intersect(SDL_Rect camera, Entity *ent);
void Camera_IntersectAll(Entity *ent);

#endif