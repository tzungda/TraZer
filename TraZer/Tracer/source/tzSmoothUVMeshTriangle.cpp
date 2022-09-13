
#include "../include/tzConstants.h"
#include "../include/tzSmoothUVMeshTriangle.h"
						
//===================================================================================
tzSmoothUVMeshTriangle::tzSmoothUVMeshTriangle(void)
	: tzSmoothMeshTriangle()
{}

//===================================================================================
tzSmoothUVMeshTriangle::tzSmoothUVMeshTriangle(std::shared_ptr<tzMesh> _mesh_ptr, const int i0, const int i1, const int i2)
	: tzSmoothMeshTriangle(_mesh_ptr, i0, i1, i2)
{}

//===================================================================================
std::shared_ptr<tzIGeometricObject> tzSmoothUVMeshTriangle::clone (void) const 
{
	return (std::make_shared< tzSmoothUVMeshTriangle >(*this));
}

//===================================================================================
tzSmoothUVMeshTriangle::tzSmoothUVMeshTriangle(const tzSmoothUVMeshTriangle& fmt)
	: tzSmoothMeshTriangle(fmt)
{}

//===================================================================================
tzSmoothUVMeshTriangle& tzSmoothUVMeshTriangle::operator= (const tzSmoothUVMeshTriangle& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzSmoothMeshTriangle::operator= (rhs);
	
	return (*this);
}

//===================================================================================
tzSmoothUVMeshTriangle::~tzSmoothUVMeshTriangle(void) {}

//===================================================================================
bool tzSmoothUVMeshTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr)  
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
	sr.mNormal 			= interpolateNormal((float)beta, (float)gamma); // for smooth shading
	sr.mU				= interpolate_u((float)beta, (float)gamma );
	sr.mV				= interpolate_v((float)beta, (float)gamma );
	sr.mLocalHitPoint 	= ray.mOrigin + t * ray.mDirection;
	
	return (true);	
}  

