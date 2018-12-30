
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
tzSmoothTriangle::interpolate_normal(const float beta, const float gamma) const {
	tzNormal normal((1 - beta - gamma) * n0 + beta * n1 + gamma * n2);
	normal.normalize();
	
	return(normal);
}


//---------------------------------------------------------------- get_bounding_box

tzBBox
tzSmoothTriangle::get_bounding_box(void) {
	double delta = 0.0001; 
	
	return(tzBBox(fmin(fmin(v0.x, v1.x), v2.x) - delta, fmax(fmax(v0.x, v1.x), v2.x) + delta, 
				fmin(fmin(v0.y, v1.y), v2.y) - delta, fmax(fmax(v0.y, v1.y), v2.y) + delta, 
				fmin(fmin(v0.z, v1.z), v2.z) - delta, fmax(fmax(v0.z, v1.z), v2.z) + delta));
}



// ------------------------------------------------------------------------------ hit

bool 
tzSmoothTriangle::hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const {	
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;
	
	double inv_denom  = 1.0 / (a * m + b * q + c * s);
	
	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	
	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;
	
	if (gamma < 0.0)
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;
	
	if (t < kEpsilon)
		return (false);
					
	tmin 				= t;
	sr.mNormal 			= interpolate_normal(beta, gamma); 
	sr.mLocalHitPoint 	= ray.o + t * ray.d;	
	
	return (true);	
}  	

	

// ------------------------------------------------------------------------------ shadow_hit
// Hit function for shadow rays

bool 																						 
tzSmoothTriangle::shadow_hit(const tzRay& ray, double& tmin) const {	
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;
	
	double inv_denom  = 1.0 / (a * m + b * q + c * s);
	
	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	 	
	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;
	
	if (gamma < 0.0)
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;
	
	if (t < kEpsilon)
		return (false);
					
	tmin = t;
	
	return(true);	
}  
