#include "Vector.h"
#include "math.h"

Vec2d vec_2d(float x, float y);
Vec3d vec_3d(float x, float y, float z);
Vec4d vec_4d(float x, float y, float z, float w);

void vec2d_Normalize(Vec2d *V);
void vec2d_Normalize(Vec3d *V);
void vec4d_Normalize(Vec4d *V);

void vec2d_CrossProduct(Vec2d *vec_Out, Vec2d vec1, Vec2d vec2)
{
	vec_Out->x = vec1.y*vec2.x;
	vec_Out->y = -(vec1.x*vec2.y);
}
void vec3d_CrossProduct(Vec3d *vec_Out, Vec3d vec1, Vec3d vec2)
{
	vec_Out->x = (vec1.y*vec2.z) - (vec1.z*vec2.y);
	vec_Out->y = (vec1.z*vec2.x) - (vec1.x*vec2.z);
	vec_Out->z = (vec1.x*vec2.y) - (vec1.y*vec2.x);
}
void vec4d_CrossProduct(Vec4d *vec_Out, Vec4d vec1, Vec4d vec2)
{
	vec_Out->x = 2*((vec1.y*vec2.z) - (vec1.w*vec2.z)) + (vec1.z*vec2.z);
	vec_Out->y = 2*((vec1.z*vec2.w) - (vec1.x*vec2.w)) + (vec1.z*vec2.w);
	vec_Out->z = 2*((vec1.w*vec2.x) - (vec1.y*vec2.x)) - (vec1.w*vec2.x);
	vec_Out->w = 2*((vec1.x*vec2.y) - (vec1.z*vec2.y)) - (vec1.x*vec2.y);
}
void vec3d_AngleToVector(Vec3d angles, Vec3d *forward, Vec3d *right, Vec3d *up)
{
	float angle;
	float sr, sp, sy, cr, cp, cy;
  
	angle = angles.x * (BRETT_DEGTORAD);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles.y * (BRETT_DEGTORAD);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles.z * (BRETT_DEGTORAD);
	sr = sin(angle);
	cr = cos(angle);
  
	if(forward)
	{
	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
	}
	if(right)
	{
	right->x = (-1*sr*sp*cy+-1*cr*-sy);
	right->y = (-1*sr*sp*sy+-1*cr*cy);
	right->z = -1*sr*cp;
	}
	if(up)
	{
	up->x = (cr*sp*cy+-sr*-sy);
	up->y = (cr*sp*sy+-sr*cy);
	up->z = cr*cp;
	}
}
float vec2d_GetLength(Vec2d vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}
float vec3d_GetLength(Vec3d vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.y*vec.y);
}
float vec4d_GetLength(Vec4d vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
}
void vec2d_Normalize(Vec2d *vec)
{
	float length;

	if (!vec)return;

	length = vec2d_GetLength (*vec);

	if (length == 0.0f)return;

	length = 1/length;

	vec->x *= length;
	vec->y *= length;
}
void vec3d_Normalize(Vec3d *vec)
{
	float length;

	if (!vec)return;

	length = vec3d_GetLength (*vec);

	if (length == 0.0f)return;

	length = 1/length;

	vec->x *= length;
	vec->y *= length;
}
void vec4d_Normalize(Vec4d *vec)
{
	float length;

	if (!vec)return;

	length = vec4d_GetLength (*vec);

	if (length == 0.0f)return;

	length = 1/length;

	vec->x *= length;
	vec->y *= length;
}
void vec3d_SetAngle(Vec3d *vec_Out, float rad)
{
	if(!vec_Out)return;
	vec_Out->x = cos(rad);
	vec_Out->y = sin(rad);
}
void vec4d_SetAngle(Vec4d *vec_Out, float rad)
{
	if(!vec_Out)return;
	vec_Out->x = cos(rad);
	vec_Out->y = sin(rad);
}
void vec3d_RotateAboutX(Vec3d *vec, float angle)
{
	Vec3d temp;
	if (!vec)return;
  
	angle = angle * BRETT_DEGTORAD;
  
	temp.x=vec->x;
	temp.y=(vec->y*cos(angle))-(vec->z*sin(angle));
	temp.z=(vec->y*sin(angle))+(vec->z*cos(angle));
  
	vec->x=temp.x;
	vec->y=temp.y;
	vec->z=temp.z;
}
