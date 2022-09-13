#ifndef TZ_VECTOR_3D
#define TZ_VECTOR_3D


#include "tzMatrix.h"
 
class tzNormal;
class tzPoint3D;

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

		tzVector3D& operator= (const tzVector3D& rhs);
		
		tzVector3D& operator= (const tzNormal& rhs);
		
		tzVector3D& operator= (const tzPoint3D& rhs); 
		
		tzVector3D operator- (void) const;									
				
		float length(void);
		
		float len_squared(void);
		
		tzVector3D operator* (const float a) const;

		tzVector3D operator* (const tzMatrix &m) const;
				
		tzVector3D operator/ (const float a) const;
		
		tzVector3D operator+ (const tzVector3D& v) const;
		
		tzVector3D& operator+= (const tzVector3D& v);
		tzVector3D& operator-= (const tzVector3D& v);
		
		tzVector3D operator- (const tzVector3D& v) const;

		float operator* (const tzVector3D& b) const;
		
		tzVector3D operator^ (const tzVector3D& v) const;
		
		void normalize(void); 
		
		tzVector3D& hat(void);

		tzMatrix rotationMatrixToV( const tzVector3D& v ) const;
};


// inlined member functions
//
//===================================================================================
inline tzVector3D tzVector3D::operator- (void) const 
{
	return (tzVector3D(-x, -y, -z));    
}

//===================================================================================
inline float tzVector3D::len_squared(void) 
{
	return (x * x + y * y + z * z);
}

//===================================================================================
inline tzVector3D tzVector3D::operator* (const float a) const 
{
	return (tzVector3D(x * a, y * a, z * a));
}

//===================================================================================
inline tzVector3D tzVector3D::operator/ (const float a) const 
{
	return (tzVector3D(x / a, y / a, z / a));
}

//===================================================================================
inline tzVector3D tzVector3D::operator+ (const tzVector3D& v) const 
{
	return (tzVector3D(x + v.x, y + v.y, z + v.z));
}

//===================================================================================
inline tzVector3D tzVector3D::operator- (const tzVector3D& v) const 
{
	return (tzVector3D(x - v.x, y - v.y, z - v.z));
}

//===================================================================================
inline float tzVector3D::operator* (const tzVector3D& v) const 
{
	return (x * v.x + y * v.y + z * v.z);
} 

//===================================================================================
inline tzVector3D tzVector3D::operator^ (const tzVector3D& v) const 
{
	return (tzVector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}

//===================================================================================
inline tzVector3D& tzVector3D::operator+= (const tzVector3D& v)
{
	x += v.x; y += v.y; z += v.z;
	return (*this);
}

//===================================================================================
inline tzVector3D& tzVector3D::operator-= (const tzVector3D& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return (*this);
}


// inlined non-member function
//
//===================================================================================
tzVector3D 											// prototype
operator* (const float a, const tzVector3D& v);

inline tzVector3D
operator* (const float a, const tzVector3D& v) {
	return (tzVector3D(a * v.x, a * v.y, a * v.z));
}


#endif



