

#include "../include/tzSphere.h"
#include "math.h"

const double tzSphere::kEpsilon = 0.001;
					
// ---------------------------------------------------------------- default constructor

tzSphere::tzSphere(void)	
	: 	tzIGeometricObject(),
		center(0.0),
		radius(1.0)
{}


// ---------------------------------------------------------------- constructor

tzSphere::tzSphere(tzPoint3D c, double r)
	: 	tzIGeometricObject(),
		center(c),
		radius(r)
{}


// ---------------------------------------------------------------- clone

tzSphere* tzSphere::clone(void) const {
	return (new tzSphere(*this));
}


// ---------------------------------------------------------------- copy constructor

tzSphere::tzSphere (const tzSphere& tzSphere)
	: 	tzIGeometricObject(tzSphere),
		center(tzSphere.center),
		radius(tzSphere.radius)
{}



// ---------------------------------------------------------------- assignment operator

tzSphere& 
tzSphere::operator= (const tzSphere& rhs)		
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);

	center 	= rhs.center;
	radius	= rhs.radius;

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzSphere::~tzSphere(void) {}

bool tzSphere::shadow_hit(const tzRay &ray, float &tmin) const
{
	double 		t;
	tzVector3D	temp = ray.o - center;
	double 		a = ray.d * ray.d;
	double 		b = 2.0 * temp * ray.d;
	double 		c = temp * temp - radius * radius;
	double 		disc = b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = (float)t;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = (float)t;
			return (true);
		}
	}

	return false;
}

//---------------------------------------------------------------- hit

bool tzSphere::hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const {
	double 		t;
	tzVector3D	temp 	= ray.o - center;
	double 		a 		= ray.d * ray.d;
	double 		b 		= 2.0 * temp * ray.d;
	double 		c 		= temp * temp - radius * radius;
	double 		disc	= b * b - 4.0 * a * c;
	
	if (disc < 0.0)
		return(false);
	else {	
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal 	 = (temp + t * ray.d) / radius;
			sr.mLocalHitPoint = ray.o + t * ray.d;
			return (true);
		} 
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal = (temp + t * ray.d) / radius;
			sr.mLocalHitPoint = ray.o + t * ray.d;
			return (true);
		} 
	}
	
	return (false);
}







