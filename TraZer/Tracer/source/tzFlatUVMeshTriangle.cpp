

#include "../include/tzConstants.h"
#include "../include/tzFlatUVMeshTriangle.h"
#include "../include/tzITexture.h"

//===================================================================================
tzFlatUVMeshTriangle::tzFlatUVMeshTriangle(void)
	: tzFlatMeshTriangle()
{}


//===================================================================================
tzFlatUVMeshTriangle::tzFlatUVMeshTriangle(std::shared_ptr<tzMesh> _mesh_ptr, const int i0, const int i1, const int i2)
	: tzFlatMeshTriangle(_mesh_ptr, i0, i1, i2)
{}

//===================================================================================
tzFlatUVMeshTriangle::tzFlatUVMeshTriangle(std::shared_ptr<tzMesh> _meshPtr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2)
	: tzFlatMeshTriangle(_meshPtr, v0, v1, v2, n0, n1, n2, uv0, uv1, uv2)
{
}

//===================================================================================
std::shared_ptr<tzIGeometricObject> tzFlatUVMeshTriangle::clone(void) const
{
	return (std::make_shared< tzFlatUVMeshTriangle >(*this));
}


//===================================================================================
tzFlatUVMeshTriangle::tzFlatUVMeshTriangle(const tzFlatUVMeshTriangle& fmt)
	: tzFlatMeshTriangle(fmt)
{}


//===================================================================================
tzFlatUVMeshTriangle& tzFlatUVMeshTriangle::operator= (const tzFlatUVMeshTriangle& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzFlatMeshTriangle::operator= (rhs);
	
	return (*this);
}


//===================================================================================
tzFlatUVMeshTriangle::~tzFlatUVMeshTriangle(void) {}


//===================================================================================
bool tzFlatUVMeshTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr)  
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
					
	tmin 				= t;
	sr.mNormal 			= mNormal;  				// for flat shading
	sr.mLocalHitPoint 	= ray.mOrigin + t * ray.mDirection;
	sr.mU = interpolate_u( (float)beta, (float)gamma );
	sr.mV = interpolate_v( (float)beta, (float)gamma );

	if (mAlphaTexture)
	{
		tzColor c = mAlphaTexture->getColor(sr);
		if (c.a < kEpsilon)
		{
			return false;
		}
	}

	tzVector3D norm( mNormal.x, mNormal.y, mNormal.z );
	if ( norm*ray.mDirection < 0.0f )
	{
		sr.mNormal = mNormal;
	}
	else
	{
		sr.mNormal = -mNormal;
	}
	
	return (true);	
}  

//===================================================================================
bool tzFlatUVMeshTriangle::shadowHit(const tzRay& ray, const tzShadeRec& sr, float& tmin) const
{
	tzPoint3D v0(mMeshPtr->mVertices[mIndexV0]);
	tzPoint3D v1(mMeshPtr->mVertices[mIndexV1]);
	tzPoint3D v2(mMeshPtr->mVertices[mIndexV2]);

	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.mDirection.x, d = v0.x - ray.mOrigin.x;
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.mDirection.y, h = v0.y - ray.mOrigin.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.mDirection.z, l = v0.z - ray.mOrigin.z;

	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;

	float inv_denom = 1.0f / (a * m + b * q + c * s);

	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;

	if (beta < 0.0f)
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

	// check alpha texture
	tzShadeRec tmpSR = sr;
	tmpSR.mU = interpolate_u((float)beta, (float)gamma);
	tmpSR.mV = interpolate_v((float)beta, (float)gamma);
	if (mAlphaTexture)
	{
		tzColor c = mAlphaTexture->getColor( tmpSR );
		if (c.a < kEpsilon)
		{
			return false;
		}
	}

	tmin = (float)t;

	return (true);
}

