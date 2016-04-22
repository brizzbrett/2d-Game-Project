#include "Camera.h"

static SDL_Rect _Camera = {0,0,1000,703};

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
SDL_Rect Camera_Bounds()
{
	SDL_Rect bounds;
	bounds.x = _Camera.x+130;
	bounds.y = _Camera.y+150;
	bounds.w = 745;
	bounds.h = 400;
	return bounds;
}
int Camera_Intersect(Entity *ent)
{
	SDL_Rect bB;
	if(!ent)
	{
		return 0;
	}
	bB = rect(ent->pos.x+ent->bounds.x,ent->pos.y+ent->bounds.y,ent->bounds.w, ent->bounds.h);
	if(rect_intersect(Camera_Bounds(), bB))
	{
		return 1;
	}
	return 0;
}