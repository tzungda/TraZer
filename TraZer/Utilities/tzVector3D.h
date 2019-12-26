#ifndef TZ_VECTOR_3D
#define TZ_VECTOR_3D


#include "tzMatrix.h"
 
class tzNormal;
class tzPoint3D;

//----------------------------------------- class Vector3D

class tzVector3D {
	public:
	
		float	x, y, z;

	public:
	
		tzVector3D(void);											// default constructor
		tzVector3D(float a);										// constructor
		tzVector3D(float _x, float _y, float _z);				// constructor
		tzVector3D(const tzVector3D& v);							// copy constructor
		tzVector3D(const tzNormal& n);								// constructs a vector from a Normal
		tzVector3D(const tzPoint3D& p);								// constructs a vector from a point

		~tzVector3D(void);										// destructor

		tzVector3D& 												// assignment operator
		operator= (const tzVector3D& rhs);
		
		tzVector3D& 												// assign a Normal to a vector
		operator= (const tzNormal& rhs);
		
		tzVector3D& 												// assign a Point3D to a vector
		operator= (const tzPoint3D& rhs); 
		
		tzVector3D												// unary minus
		operator- (void) const;									
				
		float													// length
		length(void);
		
		float													// square of the length
		len_squared(void);
		
		tzVector3D operator* (const float a) const;

		tzVector3D operator* (const tzMatrix &m) const;
				
		tzVector3D												// division by a float
		operator/ (const float a) const;
		
		tzVector3D												// addition
		operator+ (const tzVector3D& v) const;
		
		tzVector3D& operator+= (const tzVector3D& v);
		tzVector3D& operator-= (const tzVector3D& v);
		
		tzVector3D												// subtraction
		operator- (const tzVector3D& v) const;

		
		float 													// dot product							
		operator* (const tzVector3D& b) const;
		
		tzVector3D 												// cross product				
		operator^ (const tzVector3D& v) const;
		
		void 													// convert vector to a unit vector
		normalize(void); 
		
		tzVector3D& 												// return a unit vector, and normalize the vector												
		hat(void);

		tzMatrix rotationMatrixToV( const tzVector3D& v ) const;
};


// inlined member functions

// ------------------------------------------------------------------------ unary minus
// this does not change the current vector
// this allows ShadeRec objects to be declared as constant arguments in many shading
// functions that reverse the direction of a ray that's stored in the ShadeRec object

inline tzVector3D
tzVector3D::operator- (void) const {
	return (tzVector3D(-x, -y, -z));    
}


// ---------------------------------------------------------------------  len_squared
// the square of the length

inline float													
tzVector3D::len_squared(void) {
	return (x * x + y * y + z * z);
}


// ----------------------------------------------------------------------- operator*
// multiplication by a float on the right

inline tzVector3D
tzVector3D::operator* (const float a) const {
	return (tzVector3D(x * a, y * a, z * a));
}

// ----------------------------------------------------------------------- operator/
// division by a float

inline tzVector3D
tzVector3D::operator/ (const float a) const {
	return (tzVector3D(x / a, y / a, z / a));
}


// ----------------------------------------------------------------------- operator+
// addition

inline tzVector3D
tzVector3D::operator+ (const tzVector3D& v) const {
	return (tzVector3D(x + v.x, y + v.y, z + v.z));
}


// ----------------------------------------------------------------------- operator-
// subtraction

inline tzVector3D
tzVector3D::operator- (const tzVector3D& v) const {
	return (tzVector3D(x - v.x, y - v.y, z - v.z));
}


// ----------------------------------------------------------------------- operator*
// dot product

inline float 
tzVector3D::operator* (const tzVector3D& v) const {
	return (x * v.x + y * v.y + z * v.z);
} 


// ----------------------------------------------------------------------- operator^
// cross product

inline tzVector3D
tzVector3D::operator^ (const tzVector3D& v) const {
	return (tzVector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}


// ---------------------------------------------------------------------  operator+=
// compound addition

inline tzVector3D& tzVector3D::operator+= (const tzVector3D& v)
{
	x += v.x; y += v.y; z += v.z;
	return (*this);
}

inline tzVector3D& tzVector3D::operator-= (const tzVector3D& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return (*this);
}


// inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a float on the left

tzVector3D 											// prototype
operator* (const float a, const tzVector3D& v);

inline tzVector3D
operator* (const float a, const tzVector3D& v) {
	return (tzVector3D(a * v.x, a * v.y, a * v.z));
}



// non-inlined non-member function

// ----------------------------------------------------------------------- operator* 
// multiplication by a matrix on the left
/*
tzVector3D 											// prototype
operator* (const tzMatrix& mat, const tzVector3D& v);
*/


#endif



