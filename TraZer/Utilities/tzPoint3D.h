#ifndef TZ_POINT3D
#define TZ_POINT3D

#include "tzMatrix.h"
#include "tzVector3D.h"

class tzPoint3D {
	public:
	
		double x, y, z;
	
		tzPoint3D();													// default constructor
		tzPoint3D(const double a);									// constructor
		tzPoint3D(const double a, const double b, const double c);	// constructor
		tzPoint3D(const tzPoint3D& p);									// copy constructor
		~tzPoint3D();													// destructor
		
		tzPoint3D& 													// assignment operator
		operator= (const tzPoint3D& p);
		
		tzPoint3D 													// unary minus
		operator- (void) const;
	
		tzVector3D 													// vector joining two points
		operator- (const tzPoint3D& p) const;
		
		tzPoint3D 													// addition of a vector				
		operator+ (const tzVector3D& v) const;
		
		tzPoint3D 													// subtraction of a vector
		operator- (const tzVector3D& v) const;
				
		tzPoint3D 													// multiplication by a double on the right
		operator* (const double a) const;
		
		double														// square of distance bertween two points
		d_squared(const tzPoint3D& p) const;
		
		double														// distance bewteen two points
		distance(const tzPoint3D& p) const;
};



// inlined member functions

// -------------------------------------------------------------- operator-
// unary minus

inline tzPoint3D
tzPoint3D::operator- (void) const {
	return (tzPoint3D(-x, -y, -z));
}


// -------------------------------------------------------------- operator-
// the vector that joins two points

inline tzVector3D
tzPoint3D::operator- (const tzPoint3D& p) const {
	return (tzVector3D(x - p.x,y - p.y,z - p.z));
}


// -------------------------------------------------------------- operator+
// addition of a vector to a point that returns a new point

inline tzPoint3D
tzPoint3D::operator+ (const tzVector3D& v) const {
	return (tzPoint3D(x + v.x, y + v.y, z + v.z));
}


// -------------------------------------------------------------- operator-
// subtraction of a vector from a point that returns a new point

inline tzPoint3D
tzPoint3D::operator- (const tzVector3D& v) const {
	return (tzPoint3D(x - v.x, y - v.y, z - v.z));
}


// -------------------------------------------------------------- operator*
// mutliplication by a double on the right

inline tzPoint3D
tzPoint3D::operator* (const double a) const {
	return (tzPoint3D(x * a,y * a,z * a));
}


// -------------------------------------------------------------- d_squared
// the square of the distance between the two points as a member function

inline double
tzPoint3D::d_squared(const tzPoint3D& p) const {
	return (	(x - p.x) * (x - p.x) 
			+ 	(y - p.y) * (y - p.y)
			+	(z - p.z) * (z - p.z) );
}




// inlined non-member function

// -------------------------------------------------------------- operator*
// multiplication by a double on the left

tzPoint3D												// prototype
operator* (double a, const tzPoint3D& p);

inline tzPoint3D
operator* (double a, const tzPoint3D& p) {
	return (tzPoint3D(a * p.x, a * p.y, a * p.z));
}



// non-inlined non-member function

// -------------------------------------------------------------- operator*
// multiplication by a matrix on the left

tzPoint3D 											// prototype					
operator* (const tzMatrix& mat, const tzPoint3D& p);

#endif

