
#include "../include/tzConstants.h"
#include "../include/tzSmoothUVMeshTriangle.h"
						

// ----------------------------------------------------------------  default constructor

tzSmoothUVMeshTriangle::tzSmoothUVMeshTriangle(void)
	: tzSmoothMeshTriangle()
{}


// ---------------------------------------------------------------- constructor

tzSmoothUVMeshTriangle::tzSmoothUVMeshTriangle(tzMesh* _mesh_ptr, const int i0, const int i1, const int i2)
	: tzSmoothMeshTriangle(_mesh_ptr, i0, i1, i2)
{}


// ---------------------------------------------------------------- clone

tzSmoothUVMeshTriangle*
tzSmoothUVMeshTriangle::clone (void) const {
	return (new tzSmoothUVMeshTriangle(*this));
}


// ---------------------------------------------------------------- copy constructor

tzSmoothUVMeshTriangle::tzSmoothUVMeshTriangle(const tzSmoothUVMeshTriangle& fmt)
	: tzSmoothMeshTriangle(fmt)
{}


// ---------------------------------------------------------------- assignment operator

tzSmoothUVMeshTriangle&
tzSmoothUVMeshTriangle::operator= (const tzSmoothUVMeshTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	tzSmoothMeshTriangle::operator= (rhs);
	
	return (*this);
}


// ---------------------------------------------------------------- destructor

tzSmoothUVMeshTriangle::~tzSmoothUVMeshTriangle(void) {}



// ---------------------------------------------------------------- hit

bool 															 
tzSmoothUVMeshTriangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const {
	tzPoint3D v0(mesh_ptr->vertices[indexV0]);
	tzPoint3D v1(mesh_ptr->vertices[indexV1]);
	tzPoint3D v2(mesh_ptr->vertices[indexV2]);
	
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
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
	sr.mNormal 			= interpolate_normal((float)beta, (float)gamma); // for smooth shading
	sr.u				= interpolate_u((float)beta, (float)gamma );
	sr.v				= interpolate_v((float)beta, (float)gamma );
	sr.mLocalHitPoint 	= ray.o + t * ray.d;	
	
	return (true);	
}  

