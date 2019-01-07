#ifndef TZ_POINT3D
#define TZ_POINT3D

#include "tzMatrix.h"
#include "tzVector3D.h"

class tzPoint3D {
	public:
	
		float x, y, z;
	
		tzPoint3D();													// default constructor
		tzPoint3D(const float a);									// constructor
		tzPoint3D(const float a, const float b, const float c);	// constructor
		tzPoint3D(const tzPoint3D& p);									// copy constructor
		~tzPoint3D();													// destructor
		
		tzPoint3D&  operator= (const tzPoint3D& p);
		
		tzPoint3D  operator- (void) const;
	
		tzVector3D operator- (const tzPoint3D& p) const;
		
		tzPoint3D  operator+ (const tzVector3D& v) const;
		
		tzPoint3D  operator- (const tzVector3D& v) const;
				
		tzPoint3D  operator* (const float a) const;

		tzPoint3D operator* (const tzMatrix& mat) const;
		
		float d_squared(const tzPoint3D& p) const;
		
		float distance(const tzPoint3D& p) const;

};



//===================================================================================
inline tzPoint3D tzPoint3D::operator- (void) const 
{
	return (tzPoint3D(-x, -y, -z));
}


//===================================================================================
inline tzVector3D tzPoint3D::operator- (const tzPoint3D& p) const 
{
	return (tzVector3D(x - p.x,y - p.y,z - p.z));
}


//===================================================================================
inline tzPoint3D tzPoint3D::operator+ (const tzVector3D& v) const 
{
	return (tzPoint3D(x + v.x, y + v.y, z + v.z));
}


//===================================================================================
inline tzPoint3D tzPoint3D::operator- (const tzVector3D& v) const 
{
	return (tzPoint3D(x - v.x, y - v.y, z - v.z));
}


//===================================================================================
inline tzPoint3D tzPoint3D::operator* (const float a) const 
{
	return (tzPoint3D(x * a,y * a,z * a));
}

//===================================================================================
inline float tzPoint3D::d_squared(const tzPoint3D& p) const 
{
	return (	(x - p.x) * (x - p.x) 
			+ 	(y - p.y) * (y - p.y)
			+	(z - p.z) * (z - p.z) );
}


//===================================================================================
tzPoint3D operator* (float a, const tzPoint3D& p);

inline tzPoint3D operator* (float a, const tzPoint3D& p) 
{
	return (tzPoint3D(a * p.x, a * p.y, a * p.z));
}


#endif

