

#include "../include/tzSphere.h"
#include "math.h"

const float tzSphere::mEpsilon = 0.001f;
					
//===================================================================================
tzSphere::tzSphere(void)	
	: 	tzIGeometricObject(),
		mCenter(0.0),
		mRadius(1.0)
{}

//===================================================================================
tzSphere::tzSphere(tzPoint3D c, float r)
	: 	tzIGeometricObject(),
		mCenter(c),
		mRadius(r)
{}

//===================================================================================
std::shared_ptr<tzIGeometricObject> tzSphere::clone(void) const 
{
	return (std::make_shared< tzSphere >(*this));
}

//===================================================================================
tzSphere::tzSphere (const tzSphere& tzSphere)
	: 	tzIGeometricObject(tzSphere),
		mCenter(tzSphere.mCenter),
		mRadius(tzSphere.mRadius)
{}

//===================================================================================
tzSphere&  tzSphere::operator= (const tzSphere& rhs)		
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);

	mCenter = rhs.mCenter;
	mRadius = rhs.mRadius;

	return (*this);
}

//===================================================================================
tzSphere::~tzSphere(void) 
{
}

//===================================================================================
bool tzSphere::shadowHit(const tzRay &ray, const tzShadeRec& sr, float &tmin) const
{
	float 		t;
	tzVector3D	temp = ray.mOrigin - mCenter;
	float 		a = ray.mDirection * ray.mDirection;
	float 		b = 2.0f * temp * ray.mDirection;
	float 		c = temp * temp - mRadius * mRadius;
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

//===================================================================================
bool tzSphere::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) 
{
	float 		t;
	tzVector3D	temp 	= ray.mOrigin - mCenter;
	float 		a 		= ray.mDirection * ray.mDirection;
	float 		b 		= 2.0f * temp * ray.mDirection;
	float 		c 		= temp * temp - mRadius * mRadius;
	float 		disc	= b * b - 4.0f * a * c;
	
	if (disc < 0.0)
		return(false);
	else {	
		float e = sqrtf(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal 	 = (temp + t * ray.mDirection) / mRadius;
			sr.mLocalHitPoint = ray.mOrigin + t * ray.mDirection;
			return (true);
		} 
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal = (temp + t * ray.mDirection) / mRadius;
			sr.mLocalHitPoint = ray.mOrigin + t * ray.mDirection;
			return (true);
		} 
	}
	
	return (false);
}








