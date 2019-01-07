

#include <math.h>

#include "tzNormal.h"


//===================================================================================
tzNormal::tzNormal(void)
	 : x(0.0), y(0.0), z(0.0)							
{}


//===================================================================================
tzNormal::tzNormal(float a)
	 : x(a), y(a), z(a)							
{}


//===================================================================================
tzNormal::tzNormal(float _x, float _y, float _z)	 
	: x(_x), y(_y), z(_z)
{}


//===================================================================================
tzNormal::tzNormal(const tzNormal& n)
	: x(n.x), y(n.y), z(n.z)
{}


//===================================================================================
tzNormal::tzNormal(const tzVector3D& v)
	: x(v.x), y(v.y), z(v.z)  
{}


//===================================================================================
tzNormal::~tzNormal (void) 							
{}


//===================================================================================
tzNormal& tzNormal::operator= (const tzNormal& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}


//===================================================================================
tzNormal& tzNormal::operator= (const tzVector3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


//===================================================================================
tzNormal& tzNormal::operator= (const tzPoint3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


//===================================================================================
void tzNormal::normalize(void) {	
	float length = sqrtf(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}


//===================================================================================
tzNormal tzNormal::operator* ( const tzMatrix& mat )
{
	return (tzNormal(	mat.m[0][0] * x + mat.m[1][0] * y + mat.m[2][0] * z,
					mat.m[0][1] * x + mat.m[1][1] * y + mat.m[2][1] * z,
					mat.m[0][2] * x + mat.m[1][2] * y + mat.m[2][2] * z));
}





