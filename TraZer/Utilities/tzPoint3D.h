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
		
		tzPoint3D&  operator= (const tzPoint3D& p);
		
		tzPoint3D  operator- (void) const;
	
		tzVector3D operator- (const tzPoint3D& p) const;
		
		tzPoint3D  operator+ (const tzVector3D& v) const;
		
		tzPoint3D  operator- (const tzVector3D& v) const;
				
		tzPoint3D  operator* (const double a) const;

		tzPoint3D operator* (const tzMatrix& mat) const;
		
		double d_squared(const tzPoint3D& p) const;
		
		double distance(const tzPoint3D& p) const;

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
inline tzPoint3D tzPoint3D::operator* (const double a) const 
{
	return (tzPoint3D(x * a,y * a,z * a));
}

//===================================================================================
inline double tzPoint3D::d_squared(const tzPoint3D& p) const 
{
	return (	(x - p.x) * (x - p.x) 
			+ 	(y - p.y) * (y - p.y)
			+	(z - p.z) * (z - p.z) );
}


//===================================================================================
tzPoint3D operator* (double a, const tzPoint3D& p);

inline tzPoint3D operator* (double a, const tzPoint3D& p) 
{
	return (tzPoint3D(a * p.x, a * p.y, a * p.z));
}


#endif

