#include "Vector.h"
#include "math.h"

/**
 * @brief	Vector 2D cross product because I did some math, for no REASON.
 * @param	*vec_Out		If not null, the vector that is output.
 * @param	vec1		   	The first input vector.
 * @param	vec2		   	The second input vector.
 */
void vec2d_CrossProduct(Vec2d *vec_Out, Vec2d vec1, Vec2d vec2)
{
	vec_Out->x = vec1.y*vec2.x;
	vec_Out->y = -(vec1.x*vec2.y);
}

/**
 * @brief	Vector 3D cross product.
 * @param	*vec_Out		If not null, the vector that is output.
 * @param	vec1		   	The first input vector.
 * @param	vec2		   	The second input vector.
 */
void vec3d_CrossProduct(Vec3d *vec_Out, Vec3d vec1, Vec3d vec2)
{
	vec_Out->x = (vec1.y*vec2.z) - (vec1.z*vec2.y);
	vec_Out->y = (vec1.z*vec2.x) - (vec1.x*vec2.z);
	vec_Out->z = (vec1.x*vec2.y) - (vec1.y*vec2.x);
}

/**
 * @brief	Vector 4D cross product because I did so much math, for no REASON.
 * @param	*vec_Out		If not null, the vector that is output.
 * @param	vec1		   	The first input vector.
 * @param	vec2		   	The second input vector.
 */
void vec4d_CrossProduct(Vec4d *vec_Out, Vec4d vec1, Vec4d vec2)
{
	vec_Out->x = 2*((vec1.y*vec2.z) - (vec1.w*vec2.z)) + (vec1.z*vec2.z);
	vec_Out->y = 2*((vec1.z*vec2.w) - (vec1.x*vec2.w)) + (vec1.z*vec2.w);
	vec_Out->z = 2*((vec1.w*vec2.x) - (vec1.y*vec2.x)) - (vec1.w*vec2.x);
	vec_Out->w = 2*((vec1.x*vec2.y) - (vec1.z*vec2.y)) - (vec1.x*vec2.y);
}

/**
 * @brief	Vector 3D angle to vector.
 * @param	angles		The angles.
 * @param	*forward	If not null, the forward direction.
 * @param	*right  	If not null, the right dirction.
 * @param	*up	   		If not null, the up direction.
 */
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

/**
 * @brief	Vector 2D get length.
 * @param	vec	The vector you want the length of.
 * @return	A float of the length.
 */
float vec2d_GetLength(Vec2d vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}

/**
 * @brief	Vector 3D get length.
 * @param	vec	The vector you want the length of.
 * @return	A float of the length.
 */
float vec3d_GetLength(Vec3d vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.y*vec.y);
}

/**
 * @brief	Vector 4D get length.
 * @param	vec	The vector you want the length of.
 * @return	A float of the length.
 */
float vec4d_GetLength(Vec4d vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
}

/**
 * @brief	Vector 4D normalize.
 * @param	*vec		If not null, the input vector being normalized.
 */
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

/**
 * @brief	Vector 4D normalize.
 * @param	*vec		If not null, the input vector being normalized.
 */
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

/**
 * @brief	Vector 4D normalize.
 * @param	*vec		If not null, the input vector being normalized.
 */
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

/**
 * @brief	Vector 3D set angle.
 * @param	*vec_Out			If not null, the vector that's angle is being changed.
 * @param	rad			   	The radians.
 */
void vec3d_SetAngle(Vec3d *vec_Out, float rad)
{
	if(!vec_Out)return;
	vec_Out->x = cos(rad);
	vec_Out->y = sin(rad);
}

/**
 * @brief	Vector 4D set angle.
 * @param	*vec_Out			If not null, the vector that's angle is being changed.
 * @param	rad			   	The radians.
 */
void vec4d_SetAngle(Vec4d *vec_Out, float rad)
{
	if(!vec_Out)return;
	vec_Out->x = cos(rad);
	vec_Out->y = sin(rad);
}

/**
 * @brief	Vector 3D rotate about x coordinate.
 * @param	*vec		If not null, the vector being rotated.
 * @param	angle	   	The angle of rotation.
 */
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
SDL_Rect rect(int x, int y, int w, int h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	return r;
}
int rect_intersect(SDL_Rect a, SDL_Rect b)
{
	if((a.x + a.w >= b.x) &&
		(b.x + b.w >= a.x) &&
		(a.y + a.h >= b.y) &&
		(b.y + b.h >= a.y))
		return 1;
	return 0;
}
