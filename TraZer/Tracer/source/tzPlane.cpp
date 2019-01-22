
#include "../include/tzPlane.h"

const float tzPlane::mEpsilon = 0.001f;

//===================================================================================
tzPlane::tzPlane(void)
	: 	tzIGeometricObject(),
		mPoint(0.0f),
		mNormal(0.0f, 1.0f, 0.0f)
{}

//===================================================================================
tzPlane::tzPlane(const tzPoint3D& point, const tzNormal& normal)
	:	tzIGeometricObject(),
		mPoint(point),
		mNormal(normal)
{
	mNormal.normalize();
}

//===================================================================================
tzPlane::tzPlane(const tzPlane& plane)
	:	tzIGeometricObject(plane),
	mPoint(plane.mPoint),
	mNormal(plane.mNormal) 				
{}


//===================================================================================
tzPlane* tzPlane::clone(void) const 
{
	return (new tzPlane(*this));
}


//===================================================================================
tzPlane& tzPlane::operator= (const tzPlane& rhs)
{
	
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);

	mPoint = rhs.mPoint;
	mNormal = rhs.mNormal;

	return (*this);
}


//===================================================================================
tzPlane::~tzPlane(void)
{}

//===================================================================================
bool tzPlane::shadowHit(const tzRay &ray, float &tmin) const
{
	float t = (float)((mPoint - ray.mOrigin) * mNormal / (ray.mDirection * mNormal));

	if ( t > kEpsilon )
	{
		tmin = t;
		return true;
	}

	return false;
}

//===================================================================================
bool tzPlane::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const 
{
	float t = (float)(( mPoint - ray.mOrigin) * mNormal / (ray.mDirection * mNormal));
														
	if (t > kEpsilon) {
		tmin = t;
		sr.mNormal = mNormal;
		sr.mLocalHitPoint = ray.mOrigin + t * ray.mDirection;
		
		return (true);	
	}

	return(false);
}


