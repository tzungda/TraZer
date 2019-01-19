
#include "../include/tzConstants.h"
//#include "../include/tzMaths.h"
#include "../include/tzSmoothTriangle.h"

#include <math.h>

// ----------------------------------------------------------------  default constructor

tzSmoothTriangle::tzSmoothTriangle(void)
	:	tzIGeometricObject(),
		v0(0.0), 	
		v1(0,0,1), 
		v2(1,0,0),
		n0(0, 1, 0),   	
		n1(0, 1, 0),
		n2(0, 1, 0)
{}


// ---------------------------------------------------------------- constructor

tzSmoothTriangle::tzSmoothTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c)
	:	tzIGeometricObject(),	
		v0(a),
		v1(b),
		v2(c),
		n0(0, 1, 0),  
		n1(0, 1, 0),
		n2(0, 1, 0)
{}


// ---------------------------------------------------------------- clone

tzSmoothTriangle*
tzSmoothTriangle::clone(void) const {
	return (new tzSmoothTriangle(*this));
}


// ---------------------------------------------------------------- copy constructor

tzSmoothTriangle::tzSmoothTriangle(const tzSmoothTriangle& st)
	:	tzIGeometricObject(st),
		v0(st.v1),
		v1(st.v1),
		v2(st.v2),
		n0(st.n0),
		n1(st.n1),
		n2(st.n2)
{}


// ---------------------------------------------------------------- assignment operator

tzSmoothTriangle&
tzSmoothTriangle::operator= (const tzSmoothTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	v0 = rhs.v0;
	v1 = rhs.v1;
	v2 = rhs.v2;
	n0 = rhs.n0;
	n1 = rhs.n1;
	n2 = rhs.n2;
	
	return (*this);
}


// ---------------------------------------------------------------- destructor

tzSmoothTriangle::~tzSmoothTriangle(void) {}


// ---------------------------------------------------------------- computeNormal

tzNormal 
tzSmoothTriangle::interpolateNormal(const float beta, const float gamma) const {
	tzNormal normal((1 - beta - gamma) * n0 + beta * n1 + gamma * n2);
	normal.normalize();
	
	return(normal);
}


//---------------------------------------------------------------- getBoundingBox

tzBBox
tzSmoothTriangle::getBoundingBox(void) {
	float delta = 0.0001f;
	
	return(tzBBox(fminf(fminf(v0.x, v1.x), v2.x) - delta, fmaxf(fmaxf(v0.x, v1.x), v2.x) + delta, 
				fminf(fminf(v0.y, v1.y), v2.y) - delta, fmaxf(fmaxf(v0.y, v1.y), v2.y) + delta, 
				fminf(fminf(v0.z, v1.z), v2.z) - delta, fmaxf(fmaxf(v0.z, v1.z), v2.z) + delta));
}



// ------------------------------------------------------------------------------ hit

bool 
tzSmoothTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const 
{
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.mDirection.x, d = v0.x - ray.mOrigin.x;
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.mDirection.y, h = v0.y - ray.mOrigin.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.mDirection.z, l = v0.z - ray.mOrigin.z;
		
	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float inv_denom  = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	
	float r = r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * inv_denom;
	
	if (gamma < 0.0)
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	float e3 = a * p - b * r + d * s;
	float t = e3 * inv_denom;
	
	if (t < kEpsilon)
		return (false);
					
	tmin 				= t;
	sr.mNormal 			= interpolateNormal((float)beta, (float)gamma);
	sr.mLocalHitPoint 	= ray.mOrigin + t * ray.mDirection;	
	
	return (true);	
}  	

	

// ------------------------------------------------------------------------------ shadow_hit
// Hit function for shadow rays

bool tzSmoothTriangle::shadowHit(const tzRay& ray, float& tmin) const
{
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.mDirection.x, d = v0.x - ray.mOrigin.x;
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.mDirection.y, h = v0.y - ray.mOrigin.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.mDirection.z, l = v0.z - ray.mOrigin.z;
		
	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float inv_denom  = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	 	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * inv_denom;
	
	if (gamma < 0.0)
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	float e3 = a * p - b * r + d * s;
	float t = e3 * inv_denom;
	
	if (t < kEpsilon)
		return (false);
					
	tmin = t;
	
	return(true);	
}  
