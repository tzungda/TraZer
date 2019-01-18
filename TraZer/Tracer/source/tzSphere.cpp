

#include "../include/tzSphere.h"
#include "math.h"

const float tzSphere::kEpsilon = 0.001f;
					
// ---------------------------------------------------------------- default constructor

tzSphere::tzSphere(void)	
	: 	tzIGeometricObject(),
		center(0.0),
		radius(1.0)
{}


// ---------------------------------------------------------------- constructor

tzSphere::tzSphere(tzPoint3D c, float r)
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

tzSphere::~tzSphere(void) 
{
}

bool tzSphere::shadowHit(const tzRay &ray, float &tmin) const
{
	float 		t;
	tzVector3D	temp = ray.mOrigin - center;
	float 		a = ray.mDirection * ray.mDirection;
	float 		b = 2.0f * temp * ray.mDirection;
	float 		c = temp * temp - radius * radius;
	float 		disc = b * b - 4.0f * a * c;

	if (disc < 0.0)
		return(false);
	else {
		float e = sqrtf(disc);
		float denom = 2.0f * a;
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

bool tzSphere::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const {
	float 		t;
	tzVector3D	temp 	= ray.mOrigin - center;
	float 		a 		= ray.mDirection * ray.mDirection;
	float 		b 		= 2.0f * temp * ray.mDirection;
	float 		c 		= temp * temp - radius * radius;
	float 		disc	= b * b - 4.0f * a * c;
	
	if (disc < 0.0)
		return(false);
	else {	
		float e = sqrtf(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal 	 = (temp + t * ray.mDirection) / radius;
			sr.mLocalHitPoint = ray.mOrigin + t * ray.mDirection;
			return (true);
		} 
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal = (temp + t * ray.mDirection) / radius;
			sr.mLocalHitPoint = ray.mOrigin + t * ray.mDirection;
			return (true);
		} 
	}
	
	return (false);
}








