
#include "../include/tzConstants.h"
#include "../include/tzMeshTriangle.h"
#include "../include/tzITexture.h"

#include <math.h>
						

//===================================================================================
tzMeshTriangle::tzMeshTriangle(void)
	: 	tzIGeometricObject(),
	mMeshPtr(NULL),
		mIndexV0(0), mIndexV1(0), mIndexV2(0),
		mIndexN0(0), mIndexN1(0), mIndexN2(0),
		mIndexUV0(0), mIndexUV1(0), mIndexUV2(0),
		mNormal()
{}


//===================================================================================
tzMeshTriangle::tzMeshTriangle(std::shared_ptr<tzMesh> meshPtr, const int i0, const int i1, const int i2)
	: 	tzIGeometricObject(),
		mMeshPtr(meshPtr),
		mIndexV0(i0), mIndexV1(i1), mIndexV2(i2),
		mIndexN0(i0), mIndexN1(i1), mIndexN2(i2),
		mIndexUV0(i0), mIndexUV1(i1), mIndexUV2(i2)
{}

//===================================================================================
tzMeshTriangle::tzMeshTriangle(std::shared_ptr<tzMesh> meshPtr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2)
	: tzIGeometricObject(),
	mMeshPtr(meshPtr),
	mIndexV0(v0), mIndexV1(v1), mIndexV2(v2),
	mIndexN0(n0), mIndexN1(n1), mIndexN2(n2),
	mIndexUV0(uv0), mIndexUV1(uv1), mIndexUV2(uv2)
{
}

//===================================================================================
tzMeshTriangle::tzMeshTriangle(const tzMeshTriangle& mt)
	:	tzIGeometricObject(mt),
		mMeshPtr(mt.mMeshPtr), // just the pointer
		mIndexV0(mt.mIndexV0), 
		mIndexV1(mt.mIndexV1), 
		mIndexV2(mt.mIndexV2),
		mIndexN0(mt.mIndexN0), mIndexN1(mt.mIndexN1), mIndexN2(mt.mIndexN2),
		mIndexUV0(mt.mIndexUV0), mIndexUV1(mt.mIndexUV1), mIndexUV2(mt.mIndexUV2),
		mNormal(mt.mNormal)
{}


//===================================================================================
tzMeshTriangle& tzMeshTriangle::operator= (const tzMeshTriangle& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);
	
	mMeshPtr 	= rhs.mMeshPtr; // just the pointer
	mIndexV0 		= rhs.mIndexV0;
	mIndexV1 		= rhs.mIndexV1;
	mIndexV2 		= rhs.mIndexV2;
	mIndexN0 = rhs.mIndexN0;
	mIndexN1 = rhs.mIndexN1;
	mIndexN2 = rhs.mIndexN2;
	mIndexUV0 = rhs.mIndexUV0;
	mIndexUV1 = rhs.mIndexUV1;
	mIndexUV2 = rhs.mIndexUV2;
	mNormal 		= rhs.mNormal;
	
	return (*this);
}


//===================================================================================
tzMeshTriangle::~tzMeshTriangle(void) 
{
}


//===================================================================================
void tzMeshTriangle::computeNormal(const bool reverseNormal) 
{
	mNormal = (mMeshPtr->mVertices[mIndexV1] - mMeshPtr->mVertices[mIndexV0]) ^
			 (mMeshPtr->mVertices[mIndexV2] - mMeshPtr->mVertices[mIndexV0]);
	mNormal.normalize();
	
	//normal = -normal;

	if (reverseNormal)
		mNormal = -mNormal;
}


//===================================================================================
tzNormal tzMeshTriangle::getNormal(void) const 
{
	return (mNormal);
}	


//===================================================================================
tzBBox tzMeshTriangle::getBoundingBox(void) const
{
	float delta = 0.0001f;  // to avoid degenerate bounding boxes
	
	tzPoint3D v1(mMeshPtr->mVertices[mIndexV0]);
	tzPoint3D v2(mMeshPtr->mVertices[mIndexV1]);
	tzPoint3D v3(mMeshPtr->mVertices[mIndexV2]);
	
	return(tzBBox(fmin(fmin(v1.x, v2.x), v3.x) - delta, fmax(fmax(v1.x, v2.x), v3.x) + delta, 
				fmin(fmin(v1.y, v2.y), v3.y) - delta, fmax(fmax(v1.y, v2.y), v3.y) + delta, 
				fmin(fmin(v1.z, v2.z), v3.z) - delta, fmax(fmax(v1.z, v2.z), v3.z) + delta));
}


//===================================================================================
bool tzMeshTriangle::shadowHit(const tzRay& ray, const tzShadeRec& sr, float& tmin) const
{
	tzPoint3D v0(mMeshPtr->mVertices[mIndexV0]);
	tzPoint3D v1(mMeshPtr->mVertices[mIndexV1]);
	tzPoint3D v2(mMeshPtr->mVertices[mIndexV2]);

	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.mDirection.x, d = v0.x - ray.mOrigin.x;
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.mDirection.y, h = v0.y - ray.mOrigin.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.mDirection.z, l = v0.z - ray.mOrigin.z;
		
	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float inv_denom  = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;
	
	if (beta < 0.0f)
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
                                                                                                       					
	tmin = (float)t;
	
	return (true);	
}   

//===================================================================================
float  tzMeshTriangle::interpolate_u(const float beta, const float gamma) const
{
	return( (1.0f - beta - gamma) * mMeshPtr->mUs[mIndexUV0]
				+ beta * mMeshPtr->mUs[mIndexUV1]
					+ gamma * mMeshPtr->mUs[mIndexUV2] );
}


//===================================================================================
float tzMeshTriangle::interpolate_v(const float beta, const float gamma) const 
{
	return( (1.0f - beta - gamma) * mMeshPtr->mVs[mIndexUV0]
				+ beta * mMeshPtr->mVs[mIndexUV1]
					+ gamma * mMeshPtr->mVs[mIndexUV2] );
}


