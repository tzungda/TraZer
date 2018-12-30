

#include <math.h>		// for sqrt

#include "tzNormal.h"


// ---------------------------------------------------------- default constructor

tzNormal::tzNormal(void)
	 : x(0.0), y(0.0), z(0.0)							
{}


// ---------------------------------------------------------- constructor

tzNormal::tzNormal(double a)
	 : x(a), y(a), z(a)							
{}


// ---------------------------------------------------------- constructor

tzNormal::tzNormal(double _x, double _y, double _z)	 
	: x(_x), y(_y), z(_z)
{}


// ---------------------------------------------------------- copy constructor

tzNormal::tzNormal(const tzNormal& n)
	: x(n.x), y(n.y), z(n.z)
{}


// ---------------------------------------------------------- constructor
// construct a tzNormal from a vector

tzNormal::tzNormal(const tzVector3D& v)
	: x(v.x), y(v.y), z(v.z)  
{}


// ---------------------------------------------------------- destructor

tzNormal::~tzNormal (void) 							
{}


// ----------------------------------------------------------- operator=
// assignment operator

tzNormal& 
tzNormal::operator= (const tzNormal& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}


// ------------------------------------------------------------ operator=
// assignment of a vector to a tzNormal

tzNormal& 
tzNormal::operator= (const tzVector3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ------------------------------------------------------------ operator=
// assignment of a point to a tzNormal

tzNormal& 
tzNormal::operator= (const tzPoint3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ------------------------------------------------------------ tzNormalize

void 													
tzNormal::normalize(void) {	
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}


// non-member function definition

// ---------------------------------------------------------- operator*
// multiplication by a matrix on the left

// a tzNormal is transformed by multiplying it on the left by the transpose of the upper left 3 x 3
// partition of the inverse transformation matrix

tzNormal 											
operator* (const tzMatrix& mat, const tzNormal& n) {
	return (tzNormal(	mat.m[0][0] * n.x + mat.m[1][0] * n.y + mat.m[2][0] * n.z,
					mat.m[0][1] * n.x + mat.m[1][1] * n.y + mat.m[2][1] * n.z,
					mat.m[0][2] * n.x + mat.m[1][2] * n.y + mat.m[2][2] * n.z));
}





