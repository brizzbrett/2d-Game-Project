#ifndef __VECTOR_H__
#define __VECTOR_H__


/** @brief a simple 2d vector structure */
typedef struct
{
	float x;	/**<the x component of the vector*/
	float y;	/**<the y component of the vector*/
}Vec2d;

/** @brief	a simple 3d vector structure. */
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

/** @brief a simple 4d vector structure. */
typedef struct
{
	union{
		float x;	/**<the x component of the vector*/
		float r;	/**<the red component of the color*/
	};
	union{
		float y;	/**<the x component of the vector*/
		float g;	/**<the blue component of the color*/
	};
	union{
		float z;	/**<the x component of the vector*/
		float b;	/**<the green component of the color*/
	};
	union{
		float w;	/**<the x component of the vector*/
		float a;	/**<the alpha component of the color*/
	};
}Vec4d;

typedef struct
{
	union{
		Vec2d vec2d;
		Vec3d vec3d;
		Vec4d vec4d;
	};
}Vector

/**
 * @brief	Macros that defines vec2D_Set, vec3D_Set, and vec4D_Set.
 * @param	vec	The vector being set.
 * @param	a  	The x value being set.
 * @param	b  	The y value being set.
 */
#define vec2d_Set(vec,a,b)(vec.x=(a),vec.y=(b))
#define vec3d_Set(vec,a,b,c)(vec.x=(a),vec.y=(b),vec.z=(c))
#define vec4d_Set(vec,a,b,c,d)(vec.x=(a),vec.y=(b),vec.z=(c),vec.w=(d))

/**
 * @brief	A macro that defines vector 2D add.
 * @param	a	The void to process.
 * @param	b	The void to process.
 * @param	c	The void to process.
 */
#define vec2d_Add(a,b,c)(c.x=a.x+b.x,c.y=a.y+b.y)
#define vec3d_Add(a,b,c)(c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z)
#define vec4d_Add(a,b,c)(c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z,c.w=a.w+b.w)

/**
 * @brief	A macro that defines vector 2D multiply.
 * @param	a	The void to process.
 * @param	b	The void to process.
 * @param	c	The void to process.
 */
#define vec2d_Multiply(a,b,c)(c.x=a.x*b.x,c.y=a.y*b.y)
#define vec3d_Multiply(a,b,c)(c.x=a.x*b.x,c.y=a.y*b.y,c.z=a.z*b.z)
#define vec4d_Multiply(a,b,c)(c.x=a.x*b.x,c.y=a.y*b.y,c.z=a.z*b.z,c.w=a.w*b.w)

/**
 * @brief	A macro that defines vector 2D scale.
 * @param	out   	The out.
 * @param	in	  	The in.
 * @param	factor	The factor.
 */
#define vec2d_Scale(out,in,factor)(out.x = in.x*factor, out.y = in.y*factor)
#define vec2d_Scale(out,in,factor)(out.x = in.x*factor, out.y = in.y*factor, out.z = in.z*factor)
#define vec2d_Scale(out,in,factor)(out.x = in.x*factor, out.y = in.y*factor, out.z = in.z*factor, out.w = in.w*factor)

/**
 * @brief	Vector 2D.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	A Vec2d.
 */
Vec2d vec_2d(float x, float y);

/**
 * @brief	Vector 3D.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @param	z	The z coordinate.
 * @return	A Vec3d.
 */
Vec3d vec_3d(float x, float y, float z);

/**
 * @brief	Vector 4d.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @param	z	The z coordinate.
 * @param	w	The width.
 * @return	A Vec4d.
 */
Vec4d vec_4d(float x, float y, float z, float w);

void vec2d_Normalize(Vec2d *V);
void vec2d_Normalize(Vec3d *V);
void vec4d_Normalize(Vec4d *V);

void vec2d_CrossProduct(Vec2D *out, Vec2d vec1, Vec3d vec2);
void vec3d_CrossProduct(Vec3d *out, Vec3d vec1, Vec3d vec2);




#endif