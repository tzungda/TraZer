
#include <math.h>
#include "tzPoint3D.h"


//===================================================================================
tzPoint3D::tzPoint3D()
	:x(0), y(0), z(0)
{}


//===================================================================================
tzPoint3D::tzPoint3D(const float a)
	:x(a), y(a), z(a)
{}

//===================================================================================
tzPoint3D::tzPoint3D(const float a, const float b, const float c)
	:x(a), y(b), z(c)
{}


//===================================================================================
tzPoint3D::tzPoint3D(const tzPoint3D& p)
	:x(p.x), y(p.y), z(p.z)
{}


//===================================================================================
tzPoint3D::~tzPoint3D() 
{}


//===================================================================================
tzPoint3D& tzPoint3D::operator= (const tzPoint3D& rhs) 
{
	
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}

//===================================================================================
tzPoint3D tzPoint3D::operator* (const tzMatrix& mat) const
{
	return (tzPoint3D(mat.m[0][0] * x + mat.m[1][0] * y + mat.m[2][0] * z + mat.m[3][0],
		mat.m[0][1] * x + mat.m[1][1] * y + mat.m[2][1] * z + mat.m[3][1],
		mat.m[0][2] * x + mat.m[1][2] * y + mat.m[2][2] * z + mat.m[3][2]));
}

//===================================================================================
float tzPoint3D::distance(const tzPoint3D& p) const 
{
	return (sqrtf(		(x - p.x) * (x - p.x) 
					+ 	(y - p.y) * (y - p.y)
					+	(z - p.z) * (z - p.z) ));
}




