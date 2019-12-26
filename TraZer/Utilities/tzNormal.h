#ifndef TZ_NORMAL
#define TZ_NORMAL


#include "tzMatrix.h"
#include "tzVector3D.h"
#include "tzPoint3D.h"

class tzNormal {	
	public:
	
		float	x, y, z;
				
	public:
	
		tzNormal(void);
		tzNormal(float a);
		tzNormal(float _x, float _y, float _z);
		tzNormal(const tzNormal& n);
		tzNormal(const tzVector3D& v);
		
		~tzNormal(void);

		tzNormal& operator= (const tzNormal& rhs);
		
		tzNormal& operator= (const tzVector3D& rhs);
		
		tzNormal& operator= (const tzPoint3D& rhs);
		
		tzNormal operator- (void) const;	
		
		tzNormal operator+ (const tzNormal& n) const;
		
		tzNormal& operator+= (const tzNormal& n);
		
		float operator* (const tzVector3D& v) const;
		
		tzNormal operator* (const float a) const;

		tzNormal operator* ( const tzMatrix& mat) const;
		
				
		void normalize(void);
};



//===================================================================================
inline tzNormal
tzNormal::operator- (void) const {
	return (tzNormal(-x, -y, -z));
}


//===================================================================================
inline tzNormal
tzNormal::operator+ (const tzNormal& n) const {
	return (tzNormal(x + n.x, y + n.y, z + n.z));
}


//===================================================================================
inline tzNormal&
tzNormal::operator+= (const tzNormal& n) {
	x += n.x; y += n.y; z += n.z;
    return (*this);
}


//===================================================================================
inline float
tzNormal::operator* (const tzVector3D& v) const {
	return (x * v.x + y * v.y + z * v.z);
}


//===================================================================================
inline tzNormal
tzNormal::operator* (const float a) const {
	return (tzNormal(x * a, y * a, z * a));
}



//===================================================================================
tzNormal											
operator* (const float a, const tzNormal& n);

inline tzNormal
operator*(const float f, const tzNormal& n) {
	return (tzNormal(f * n.x, f * n.y,f * n.z));
}


//===================================================================================
tzVector3D
operator+ (const tzVector3D& v, const tzNormal& n);

inline tzVector3D
operator+ (const tzVector3D& v, const tzNormal& n) {
	return (tzVector3D(v.x + n.x, v.y + n.y, v.z + n.z));
}	


//===================================================================================
tzVector3D
operator- (const tzVector3D&, const tzNormal& n);

inline tzVector3D
operator- (const tzVector3D& v, const tzNormal& n) {
	return (tzVector3D(v.x - n.x, v.y - n.y, v.z - n.z));
}


//===================================================================================
float
operator* (const tzVector3D& v, const tzNormal& n);

inline float
operator* (const tzVector3D& v, const tzNormal& n) {
	return (v.x * n.x + v.y * n.y + v.z * n.z);     
}



#endif



