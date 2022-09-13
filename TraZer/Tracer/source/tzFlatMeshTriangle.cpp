

#include "../include/tzConstants.h"
#include "../include/tzFlatMeshTriangle.h"
						

//===================================================================================
tzFlatMeshTriangle::tzFlatMeshTriangle(void)
	: 	tzMeshTriangle()
{}


//===================================================================================
tzFlatMeshTriangle::tzFlatMeshTriangle (std::shared_ptr<tzMesh> _meshPtr, const int i0, const int i1, const int i2)
	: 	tzMeshTriangle(_meshPtr, i0, i1, i2)
{}

//===================================================================================
tzFlatMeshTriangle::tzFlatMeshTriangle(std::shared_ptr<tzMesh> _meshPtr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2)
	: tzMeshTriangle(_meshPtr, v0, v1, v2, n0, n1, n2, uv0, uv1, uv2)
{
}

//===================================================================================
std::shared_ptr<tzIGeometricObject> tzFlatMeshTriangle::clone(void) const
{
	return (std::make_shared< tzFlatMeshTriangle >(*this));
}

//===================================================================================
tzFlatMeshTriangle::tzFlatMeshTriangle(const tzFlatMeshTriangle& fmt)
	:	tzMeshTriangle(fmt)
{}


//===================================================================================
tzFlatMeshTriangle& tzFlatMeshTriangle::operator= (const tzFlatMeshTriangle& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzMeshTriangle::operator= (rhs);
	
	return (*this);
}


//===================================================================================
tzFlatMeshTriangle::~tzFlatMeshTriangle(void) {}


//===================================================================================
bool tzFlatMeshTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) 
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
	
	return (true);	
}  


