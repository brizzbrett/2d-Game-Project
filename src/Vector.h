#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <SDL.h>

/**   
 * @brief	Degrees to Radians conversion.   
 */
#define BRETT_DEGTORAD	0.017453292519943295769236907684886

/**   
 * @brief	a simple 2d vector structure 
 */
typedef struct
{
	float x;	/**<the x component of the vector*/
	float y;	/**<the y component of the vector*/
}Vec2d;

/**   
 * @brief	a simple 3d vector structure. 
 */
typedef struct
{
	union{
		float x;	/**<the x component of the vector*/
		float r;	/**<the red component of the color*/
	};
	union{
		float y;	/**<the x component of the vector*/
		float g;	/**<the green component of the color*/
	};
	union{
		float z;	/**<the x component of the vector*/
		float b;	/**<the blue component of the color*/
	};
}Vec3d;

/**   
 * @brief	a simple 4d vector structure. 
 */
typedef struct
{
	union{
		float x;	/**<the x component of the vector*/
		float r;	/**<the red component of the color*/
	};
	union{
		float y;	/**<the x component of the vector*/
		float g;	/**<the green component of the color*/
	};
	union{
		float z;	/**<the x component of the vector*/
		float b;	/**<the blue component of the color*/
	};
	union{
		float w;	/**<the x component of the vector*/
		float a;	/**<the alpha component of the color*/
	};
}Vec4d;

/**
 * @brief	Sets a specified 2D Vectors x and y.
 * @param	vec	The vector being set.
 * @param	a  	The x value to be set to vec.x.
 * @param	b  	The y value to be set to vec.y.
 */
#define vec2d_Set(vec,a,b)(vec.x=(a),vec.y=(b))

/**
 * @brief	Sets a specified 3D Vectors x, y and z.
 * @param	vec	The vector being set.
 * @param	a  	The x value to be set to vec.x.
 * @param	b  	The y value to be set to vec.y.
 * @param	c  	The z value to be set to vec.z.
 */
#define vec3d_Set(vec,a,b,c)(vec.x=(a),vec.y=(b),vec.z=(c))
/**
 * @brief	Sets a specified 4D Vectors x, y, z and w.
 * @param	vec	The vector being set.
 * @param	a  	The x value to be set to vec.x.
 * @param	b  	The y value to be set to vec.y.
 * @param	c  	The z value to be set to vec.z.
 * @param	d  	The w value to be set to vec.w.
 */
#define vec4d_Set(vec,a,b,c,d)(vec.x=(a),vec.y=(b),vec.z=(c),vec.w=(d))
/**
 * @brief	A macro that defines vector 2D add.
 * @param	a	The 1st in vector to add.
 * @param	b	The 2nd in vector to add.
 * @param	c	The return vector with the new x,y
 */

#define vec2d_Add(a,b,c)(c.x=a.x+b.x,c.y=a.y+b.y)
/**
 * @brief	A macro that defines vector 3D add.
 * @param	a	The 1st in vector to add.
 * @param	b	The 2nd in vector to add.
 * @param	c	The return vector with the new x,y,z
 */
#define vec3d_Add(a,b,c)(c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z)
/**
 * @brief	A macro that defines vector 4D add.
 * @param	a	The 1st in vector to add.
 * @param	b	The 2nd in vector to add.
 * @param	c	The return vector with the new x,y,z,w
 */
#define vec4d_Add(a,b,c)(c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z,c.w=a.w+b.w)

#define vec2d_Subtract(a,b,c)(c.x=a.x-b.x,c.y=a.y-b.y)
/**
 * @brief	A macro that defines vector 3D add.
 * @param	a	The 1st in vector to add.
 * @param	b	The 2nd in vector to add.
 * @param	c	The return vector with the new x,y,z
 */
#define vec3d_Subtract(a,b,c)(c.x=a.x-b.x,c.y=a.y-b.y,c.z=a.z-b.z)
/**
 * @brief	A macro that defines vector 4D add.
 * @param	a	The 1st in vector to add.
 * @param	b	The 2nd in vector to add.
 * @param	c	The return vector with the new x,y,z,w
 */
#define vec4d_Subtract(a,b,c)(c.x=a.x-b.x,c.y=a.y-b.y,c.z=a.z-b.z,c.w=a.w-b.w)

/**
 * @brief	Defines 2D vector multiplication.
 * @param	a	The 1st in vector to multiply
 * @param	b	The 2nd in vector to multiply
 * @param	c	The return vector with the new x,y
 */

#define vec2d_Multiply(a,b,c)(c.x=a.x*b.x,c.y=a.y*b.y)
/**
 * @brief	Defines 3D vector multiplication.
 * @param	a	The 1st in vector
 * @param	b	The 2nd in vector
 * @param	c	The return vector with the new x,y,z
 */
#define vec3d_Multiply(a,b,c)(c.x=a.x*b.x,c.y=a.y*b.y,c.z=a.z*b.z)
/**
 * @brief	Defines 4D vector multiplicaton.
 * @param	a	The 1st in vector
 * @param	b	The 2nd in vector
 * @param	c	The return vector with the new x,y,z,w
 */
#define vec4d_Multiply(a,b,c)(c.x=a.x*b.x,c.y=a.y*b.y,c.z=a.z*b.z,c.w=a.w*b.w)

/**
 * @brief	Defines 2D vector scaling.
 * @param	out   	The return vector.
 * @param	in	  	The input vector.
 * @param	factor	The factor you are scaling by.
 */
#define vec2d_Scale(out,in,factor)(out.x = in.x*factor, out.y = in.y*factor)
/**
 * @brief	Defines 3D vector scaling.
 * @param	out   	The return vector.
 * @param	in	  	The input vector.
 * @param	factor	The factor you are scaling by.
 */
#define vec3d_Scale(out,in,factor)(out.x = in.x*factor, out.y = in.y*factor, out.z = in.z*factor)
/**
 * @brief	Defines 4D vector scaling.
 * @param	out   	The return vector.
 * @param	in	  	The input vector.
 * @param	factor	The factor you are scaling by.
 */
#define vec4d_Scale(out,in,factor)(out.x = in.x*factor, out.y = in.y*factor, out.z = in.z*factor, out.w = in.w*factor)

/**
 * @brief	Defines 2D vector negation.
 * @param	out	The return vector that was negated.
 * @param	in 	The input vector.
 */
#define vec2d_Negate(out,in)(out.x=(-in.x), out.y=(-in.y))
/**
 * @brief	Defines 3D vector negation.
 * @param	out	The return vector that was negated.
 * @param	in 	The input vector.
 */
#define vec3d_Negate(out,in)(out.x=(-in.x), out.y=(-in.y), out.z=(-in.z))
/**
 * @brief	Defines 4D vector negation.
 * @param	out	The return vector that was negated.
 * @param	in 	The input vector.
 */
#define vec4d_Negate(out,in)(out.x=(-in.x), out.y=(-in.y), out.z=(-in.z), out.w=(-in.w))

/**
 * @brief	Vector 2D cross product because I did some math, for no REASON.
 * @param	*vec_Out		If not null, the vector that is output.
 * @param	vec1		   	The first input vector.
 * @param	vec2		   	The second input vector.
 */
void vec2d_CrossProduct(Vec2d *vec_Out, Vec2d vec1, Vec2d vec2);
/**
 * @brief	Vector 3D cross product.
 * @param	*vec_Out		If not null, the vector that is output.
 * @param	vec1		   	The first input vector.
 * @param	vec2		   	The second input vector.
 */
void vec3d_CrossProduct(Vec3d *vec_Out, Vec3d vec1, Vec3d vec2);
/**
 * @brief	Vector 4D cross product because I did so much math, for no REASON.
 * @param	*vec_Out		If not null, the vector that is output.
 * @param	vec1		   	The first input vector.
 * @param	vec2		   	The second input vector.
 */
void vec4d_CrossProduct(Vec4d *vec_Out, Vec4d vec1, Vec4d vec2);

/**
 * @brief	Vector 3D angle to vector.
 * @param	angles		The angles.
 * @param	*forward	If not null, the forward direction.
 * @param	*right  	If not null, the right dirction.
 * @param	*up	   		If not null, the up direction.
 */
void vec3d_AngleToVector(Vec3d angles, Vec3d *forward, Vec3d *right, Vec3d *up);

/**
 * @brief	Vector 2D get length.
 * @param	vec	The vector you want the length of.
 * @return	A float of the length.
 */
float vec2d_GetLength(Vec2d vec);
/**
 * @brief	Vector 3D get length.
 * @param	vec	The vector you want the length of.
 * @return	A float of the length.
 */
float vec3d_GetLength(Vec3d vec);
/**
 * @brief	Vector 4D get length.
 * @param	vec	The vector you want the length of.
 * @return	A float of the length.
 */
float vec4d_GetLength(Vec4d vec);

/**
 * @brief	Vector 2D normalize.
 * @param  *vec		If not null, the input vector being normalized.
 */
void vec2d_Normalize(Vec2d *vec);
/**
 * @brief	Vector 3D normalize.
 * @param  *vec		If not null, the input vector being normalized.
 */
void vec3d_Normalize(Vec3d *vec);
/**
 * @brief	Vector 4D normalize.
 * @param  *vec		If not null, the input vector being normalized.
 */
void vec4d_Normalize(Vec4d *vec);

/**
 * @brief	Vector 3D set angle.
 * @param  *vec_Out			If not null, the vector that's angle is being changed.
 * @param	rad			   	The radians.
 */
void vec3d_SetAngle(Vec3d *vec_Out, float rad);
/**
 * @brief	Vector 4D set angle.
 * @param  *vec_Out			If not null, the vector that's angle is being changed.
 * @param	rad			   	The radians.
 */
void vec4d_SetAngle(Vec4d *vec_Out, float rad);

/**
 * @brief	Vector 3D rotate about x coordinate.
 * @param  *vec			If not null, the vector who's x value is being rotated around.
 * @param	angle	   	The angle of rotation.
 */
void vec3d_RotateAboutX(Vec3d *vec, float angle);

SDL_Rect rect(int x, int y, int w, int h);
int rect_intersect(SDL_Rect a, SDL_Rect b);

#endif