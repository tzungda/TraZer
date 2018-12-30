
#include <math.h>
#include "tzPoint3D.h"


// --------------------------------------------- default constructor

tzPoint3D::tzPoint3D()
	:x(0), y(0), z(0)
{}


// --------------------------------------------- constructor

tzPoint3D::tzPoint3D(const double a)
	:x(a), y(a), z(a)
{}

// --------------------------------------------- constructor

tzPoint3D::tzPoint3D(const double a, const double b, const double c)
	:x(a), y(b), z(c)
{}


// --------------------------------------------- copy constructor

tzPoint3D::tzPoint3D(const tzPoint3D& p)
	:x(p.x), y(p.y), z(p.z)
{}


// --------------------------------------------- destructor

tzPoint3D::~tzPoint3D() 
{}


// --------------------------------------------- assignment operator

tzPoint3D& 
tzPoint3D::operator= (const tzPoint3D& rhs) {
	
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}



// --------------------------------------------- distance
// distance between two points

double
tzPoint3D::distance(const tzPoint3D& p) const {
	return (sqrt(		(x - p.x) * (x - p.x) 
					+ 	(y - p.y) * (y - p.y)
					+	(z - p.z) * (z - p.z) ));
}


// non-member function

// --------------------------------------------- operator*
// multiplication by a matrix on the left

tzPoint3D 						
operator* (const tzMatrix& mat, const tzPoint3D& p) {
	return (tzPoint3D(mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2] * p.z + mat.m[0][3],
					mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2] * p.z + mat.m[1][3],
					mat.m[2][0] * p.x + mat.m[2][1] * p.y + mat.m[2][2] * p.z + mat.m[2][3]));
}


