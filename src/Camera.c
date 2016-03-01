#include "Camera.h"

static SDL_Rect _Camera = {0,0,0,0};

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