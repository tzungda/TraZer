
#include "../include/tzConstants.h"
#include "../include/tzTriangle.h"
#include <math.h>

// ----------------------------------------------------------------  default constructor

tzTriangle::tzTriangle(void)
	: 	tzIGeometricObject(),
		v0(0, 0, 0), 
		v1(0,0,1), 
		v2(1,0,0),
		normal(0, 1, 0)
{}

// ---------------------------------------------------------------- constructor

tzTriangle::tzTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c)
	: 	tzIGeometricObject(),
		v0(a),
		v1(b),
		v2(c)
{
	compute_normal();	
}


// ---------------------------------------------------------------- clone

tzTriangle*
tzTriangle::clone(void) const {
	return (new tzTriangle(*this));
}


// ---------------------------------------------------------------- copy constructor

tzTriangle::tzTriangle(const tzTriangle& triangle)
	:	tzIGeometricObject(triangle),
		v0(triangle.v0),
		v1(triangle.v1),
		v2(triangle.v2),
		normal(triangle.normal)
{}


// ---------------------------------------------------------------- assignment operator

tzTriangle&
tzTriangle::operator= (const tzTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator=(rhs);

	v0 		= rhs.v0;
	v1 		= rhs.v1;
	v2 		= rhs.v2;
	normal 	= rhs.normal;
		
	return (*this);
}


// ---------------------------------------------------------------- destructor

tzTriangle::~tzTriangle(void) {}



// ---------------------------------------------------------------- compute_normal

void 
tzTriangle::compute_normal(void) {
	normal = (v1 - v0) ^ (v2 - v0);  
	normal.normalize();
}


tzBBox
tzTriangle::get_bounding_box(void) {
	float delta = 0.000001f; 
	
	return (tzBBox(fminf(fminf(v0.x, v1.x), v2.x) - delta, fmaxf(fmaxf(v0.x, v1.x), v2.x) + delta, 
				 fminf(fminf(v0.y, v1.y), v2.y) - delta, fmaxf(fmaxf(v0.y, v1.y), v2.y) + delta, 
				 fminf(fminf(v0.z, v1.z), v2.z) - delta, fmaxf(fmaxf(v0.z, v1.z), v2.z) + delta));
}


// ------------------------------------------------------------------------------ hit

bool 
tzTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const {
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
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
	
	if (gamma < 0.0 )
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	float e3 = a * p - b * r + d * s;
	float t = e3 * inv_denom;
	
	if (t < kEpsilon) 
		return (false);
					
	tmin 				= t;
	sr.mNormal 			= normal;  	
	sr.mLocalHitPoint 	= ray.o + t * ray.d;	
	
	return (true);	
}  		


// ------------------------------------------------------------------------------ shadow_hit

bool tzTriangle::shadowHit(const tzRay& ray, float& tmin) const 
{
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
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

