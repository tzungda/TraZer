
#include "../include/tzConstants.h"
#include "../include/tzTriangle.h"
#include <math.h>

//===================================================================================
tzTriangle::tzTriangle(void)
	: 	tzIGeometricObject(),
		mV0(0, 0, 0), 
		mV1(0,0,1), 
		mV2(1,0,0),
		mNormal(0, 1, 0)
{}

//===================================================================================
tzTriangle::tzTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c)
	: 	tzIGeometricObject(),
		mV0(a),
		mV1(b),
		mV2(c)
{
	computeNormal();	
}


//===================================================================================
std::shared_ptr<tzIGeometricObject> tzTriangle::clone(void) const
{
	return (std::make_shared< tzTriangle >(*this));
}


//===================================================================================
tzTriangle::tzTriangle(const tzTriangle& triangle)
	:	tzIGeometricObject(triangle),
		mV0(triangle.mV0),
		mV1(triangle.mV1),
		mV2(triangle.mV2),
		mNormal(triangle.mNormal)
{}


//===================================================================================
tzTriangle& tzTriangle::operator= (const tzTriangle& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator=(rhs);

	mV0 = rhs.mV0;
	mV1 = rhs.mV1;
	mV2 = rhs.mV2;
	mNormal = rhs.mNormal;
		
	return (*this);
}


//===================================================================================
tzTriangle::~tzTriangle(void) {}


//===================================================================================
void tzTriangle::computeNormal(void) 
{
	mNormal = (mV1 - mV0) ^ (mV2 - mV0);  
	mNormal.normalize();
}

//===================================================================================
tzBBox tzTriangle::getBoundingBox(void) const
{
	float delta = 0.000001f; 
	
	return (tzBBox(fminf(fminf(mV0.x, mV1.x), mV2.x) - delta, fmaxf(fmaxf(mV0.x, mV1.x), mV2.x) + delta, 
				 fminf(fminf(mV0.y, mV1.y), mV2.y) - delta, fmaxf(fmaxf(mV0.y, mV1.y), mV2.y) + delta, 
				 fminf(fminf(mV0.z, mV1.z), mV2.z) - delta, fmaxf(fmaxf(mV0.z, mV1.z), mV2.z) + delta));
}


//===================================================================================
bool tzTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr)  
{
	float a = mV0.x - mV1.x, b = mV0.x - mV2.x, c = ray.mDirection.x, d = mV0.x - ray.mOrigin.x;
	float e = mV0.y - mV1.y, f = mV0.y - mV2.y, g = ray.mDirection.y, h = mV0.y - ray.mOrigin.y;
	float i = mV0.z - mV1.z, j = mV0.z - mV2.z, k = ray.mDirection.z, l = mV0.z - ray.mOrigin.z;
		
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
	sr.mNormal 			= mNormal;  	
	sr.mLocalHitPoint 	= ray.mOrigin + t * ray.mDirection;
	
	return (true);	
}  		


//===================================================================================
bool tzTriangle::shadowHit(const tzRay& ray, const tzShadeRec& sr, float& tmin) const
{
	float a = mV0.x - mV1.x, b = mV0.x - mV2.x, c = ray.mDirection.x, d = mV0.x - ray.mOrigin.x;
	float e = mV0.y - mV1.y, f = mV0.y - mV2.y, g = ray.mDirection.y, h = mV0.y - ray.mOrigin.y;
	float i = mV0.z - mV1.z, j = mV0.z - mV2.z, k = ray.mDirection.z, l = mV0.z - ray.mOrigin.z;
		
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

