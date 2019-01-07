
#include <math.h>    // for sqrt

#include "tzVector3D.h"
#include "tzNormal.h"
#include "tzPoint3D.h"

// ---------------------------------------------------------- default constructor

tzVector3D::tzVector3D(void)
	 : x(0.0), y(0.0), z(0.0)							
{}

// ---------------------------------------------------------- constructor

tzVector3D::tzVector3D(float a)
	 : x(a), y(a), z(a)							
{}

// ---------------------------------------------------------- constructor

tzVector3D::tzVector3D(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z)
{}

// ---------------------------------------------------------- copy constructor

tzVector3D::tzVector3D(const tzVector3D& vector)
	: x(vector.x), y(vector.y), z(vector.z)
{}


// ---------------------------------------------------------- constructor
// constructs a vector from a normal

tzVector3D::tzVector3D(const tzNormal& n)
	: x(n.x), y(n.y), z(n.z)
{}

// ---------------------------------------------------------- constructor
// constructs a vector from a point
// this is used in the ConcaveHemisphere hit functions

tzVector3D::tzVector3D(const tzPoint3D& p)
	: x(p.x), y(p.y), z(p.z)
{}


// ---------------------------------------------------------- destructor

tzVector3D::~tzVector3D(void) {}



// ---------------------------------------------------------- assignment operator

tzVector3D&
tzVector3D::operator= (const tzVector3D& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}

//===================================================================================
tzVector3D tzVector3D::operator* (const tzMatrix &m) const
{
	tzVector3D v;
	v.x = x * m.m[0][0] + y*m.m[1][0] + z*m.m[2][0];
	v.y = x * m.m[0][1] + y*m.m[1][1] + z*m.m[2][1];
	v.z = x * m.m[0][2] + y*m.m[1][2] + z*m.m[2][2];
	return v;
}

// ----------------------------------------------------------- assignment operator
// assign a Normal to a vector

tzVector3D&
tzVector3D::operator= (const tzNormal& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ---------------------------------------------------------- assignment operator 
// assign a point to a vector

tzVector3D&
tzVector3D::operator= (const tzPoint3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ----------------------------------------------------------  length
// length of the vector

float													
tzVector3D::length(void) {
	return (sqrtf(x * x + y * y + z * z));
}


// ----------------------------------------------------------  normalize
// converts the vector to a unit vector

void 													
tzVector3D::normalize(void) {
	float length = sqrtf(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}


// ----------------------------------------------------------  hat
// converts the vector to a unit vector and returns the vector

tzVector3D&
tzVector3D::hat(void) {
	float length = sqrtf(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return (*this);
} 


// non-member function

// ----------------------------------------------------------  operator* 
// multiplication by a matrix on the left
/*
tzVector3D
operator* (const tzMatrix& mat, const tzVector3D& v) {
	return (tzPoint3D(mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
					mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
					mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z));
}
*/
