#ifndef TZ_NORMAL
#define TZ_NORMAL


#include "tzMatrix.h"
#include "tzVector3D.h"
#include "tzPoint3D.h"

class tzNormal {	
	public:
	
		double	x, y, z;
				
	public:
	
		tzNormal(void);										// default constructor
		tzNormal(double a);									// constructor
		tzNormal(double _x, double _y, double _z);			// constructor
		tzNormal(const tzNormal& n); 							// copy constructor
		tzNormal(const tzVector3D& v);							// constructs a normal from a vector
		
		~tzNormal(void);										// destructor

		tzNormal& 											// assignment operator
		operator= (const tzNormal& rhs);
		
		tzNormal& 											// assignment of a vector to a normal
		operator= (const tzVector3D& rhs);
		
		tzNormal& 											// assignment of a point to a normal
		operator= (const tzPoint3D& rhs);
		
		tzNormal 												// unary minus
		operator- (void) const;	
		
		tzNormal 												// addition
		operator+ (const tzNormal& n) const;
		
		tzNormal& 											// compound addition
		operator+= (const tzNormal& n);
		
		double
		operator* (const tzVector3D& v) const;				// dot product with a vector on the right
		
		tzNormal 												// multiplication by a double on the right
		operator* (const double a) const;
				
		void 												// convert normal to a unit normal
		normalize(void); 									 		
};



// inlined member functions

// ----------------------------------------------------------------------- operator-
// unary minus

inline tzNormal
tzNormal::operator- (void) const {
	return (tzNormal(-x, -y, -z));
}


// ----------------------------------------------------------------------- operator+
// addition of two normals

inline tzNormal
tzNormal::operator+ (const tzNormal& n) const {
	return (tzNormal(x + n.x, y + n.y, z + n.z));
}


// ----------------------------------------------------------------------- addition
// compound addition of two normals

inline tzNormal&
tzNormal::operator+= (const tzNormal& n) {
	x += n.x; y += n.y; z += n.z;
    return (*this);
}


// ----------------------------------------------------------------------- operator*
// dot product of a normal on the left and a vector on the right

inline double
tzNormal::operator* (const tzVector3D& v) const {
	return (x * v.x + y * v.y + z * v.z);
}


// ----------------------------------------------------------------------- operator*
// multiplication by a double on the right

inline tzNormal
tzNormal::operator* (const double a) const {
	return (tzNormal(x * a, y * a, z * a));
}



// inlined non-member functions

// ----------------------------------------------------------------------- operator*
// multiplication by a double on the left

tzNormal											// prototype
operator* (const double a, const tzNormal& n);

inline tzNormal
operator*(const double f, const tzNormal& n) {
	return (tzNormal(f * n.x, f * n.y,f * n.z));
}


// ----------------------------------------------------------------------- operator+
// addition of a vector on the left to return a vector 

tzVector3D										// prototype
operator+ (const tzVector3D& v, const tzNormal& n);

inline tzVector3D
operator+ (const tzVector3D& v, const tzNormal& n) {
	return (tzVector3D(v.x + n.x, v.y + n.y, v.z + n.z));
}	


// ----------------------------------------------------------------------- operator-
// subtraction of a normal from a vector to return a vector

tzVector3D
operator- (const tzVector3D&, const tzNormal& n);

inline tzVector3D
operator- (const tzVector3D& v, const tzNormal& n) {
	return (tzVector3D(v.x - n.x, v.y - n.y, v.z - n.z));
}


// ----------------------------------------------------------------------- operator*
// dot product of a vector on the left and a normal on the right

double
operator* (const tzVector3D& v, const tzNormal& n);

inline double
operator* (const tzVector3D& v, const tzNormal& n) {
	return (v.x * n.x + v.y * n.y + v.z * n.z);     
}



// non-inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a matrix on the left

tzNormal 												// prototype							
operator* (const tzMatrix& mat, const tzNormal& n);

#endif



