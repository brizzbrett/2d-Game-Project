#include "Camera.h"

static SDL_Rect _Camera = {0,0,1600,900};

SDL_Rect Camera_GetActiveCamera()
{
	return _Camera;
}
Vec2d Camera_GetPosition()
{
	Vec2d pos;
	vec2d_Set(pos, _Camera.x, _Camera.y);
	return pos;
}
Vec2d Camera_GetSize()
{
	Vec2d size;
	vec2d_Set(size, _Camera.w, _Camera.h);
	return size;
}
void Camera_SetPosition(Vec2d pos)
{
	_Camera.x = pos.x;
	_Camera.y = pos.y;
}
void Camera_SetSize(Vec2d size)
{
	_Camera.w = size.x;
	_Camera.h = size.y;
}
int Camera_Intersect(SDL_Rect camera, Entity *ent)
{
	SDL_Rect aB, bB;
	if(!ent)
	{
		return 0;
	}
	bB = rect(ent->pos.x,ent->pos.y,ent->bounds.w, ent->bounds.h);
	if(rect_intersect(_Camera, bB))
		return 1;
	return 0;
}
void Camera_IntersectAll(Entity *entList)
{
	int i;
	if(!entList)return;

	for(i=0; i < Entity_GetNum(); i++)
	{
		if(!entList[i].inuse)
		{
			continue;
		}
		if(Camera_Intersect(_Camera, &entList[i]))
		{
			Entity_DrawAll();
		}
	}
	return;
}