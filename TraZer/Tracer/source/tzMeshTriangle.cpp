
#include "../include/tzConstants.h"
#include "../include/tzMeshTriangle.h"
#include <math.h>
						

//===================================================================================
tzMeshTriangle::tzMeshTriangle(void)
	: 	tzIGeometricObject(),
		mesh_ptr(NULL),
		indexV0(0), indexV1(0), indexV2(0),
		indexN0(0), indexN1(0), indexN2(0),
		indexUV0(0), indexUV1(0), indexUV2(0),
		normal()
{}


//===================================================================================
tzMeshTriangle::tzMeshTriangle(tzMesh* _mesh_ptr, const int i0, const int i1, const int i2)
	: 	tzIGeometricObject(),
		mesh_ptr(_mesh_ptr),
		indexV0(i0), indexV1(i1), indexV2(i2),
		indexN0(i0), indexN1(i1), indexN2(i2),
		indexUV0(i0), indexUV1(i1), indexUV2(i2)
{}

//===================================================================================
tzMeshTriangle::tzMeshTriangle(tzMesh* _mesh_ptr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2)
	: tzIGeometricObject(),
	mesh_ptr(_mesh_ptr),
	indexV0(v0), indexV1(v1), indexV2(v2),
	indexN0(n0), indexN1(n1), indexN2(n2),
	indexUV0(uv0), indexUV1(uv1), indexUV2(uv2)
{
}

//===================================================================================
tzMeshTriangle::tzMeshTriangle(const tzMeshTriangle& mt)
	:	tzIGeometricObject(mt),
		mesh_ptr(mt.mesh_ptr), // just the pointer
		indexV0(mt.indexV0), 
		indexV1(mt.indexV1), 
		indexV2(mt.indexV2),
		indexN0(mt.indexN0), indexN1(mt.indexN1), indexN2(mt.indexN2),
		indexUV0(mt.indexUV0), indexUV1(mt.indexUV1), indexUV2(mt.indexUV2),
		normal(mt.normal)
{}


//===================================================================================
tzMeshTriangle& tzMeshTriangle::operator= (const tzMeshTriangle& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);
	
	mesh_ptr 	= rhs.mesh_ptr; // just the pointer
	indexV0 		= rhs.indexV0;
	indexV1 		= rhs.indexV1;
	indexV2 		= rhs.indexV2;
	indexN0 = rhs.indexN0;
	indexN1 = rhs.indexN1;
	indexN2 = rhs.indexN2;
	indexUV0 = rhs.indexUV0;
	indexUV1 = rhs.indexUV1;
	indexUV2 = rhs.indexUV2;
	normal 		= rhs.normal;
	
	return (*this);
}


//===================================================================================
tzMeshTriangle::~tzMeshTriangle(void) 
{
	if (mesh_ptr) {
		delete mesh_ptr;
		mesh_ptr = NULL;
	}
}


//===================================================================================
void tzMeshTriangle::computeNormal(const bool reverse_normal) 
{
	normal = (mesh_ptr->vertices[indexV1] - mesh_ptr->vertices[indexV0]) ^
			 (mesh_ptr->vertices[indexV2] - mesh_ptr->vertices[indexV0]);
	normal.normalize();
	
	//normal = -normal;

	if (reverse_normal)
		normal = -normal;
}


//===================================================================================
tzNormal tzMeshTriangle::getNormal(void) const 
{
	return (normal);
}	


//===================================================================================
tzBBox tzMeshTriangle::getBoundingBox(void) 
{
	float delta = 0.0001f;  // to avoid degenerate bounding boxes
	
	tzPoint3D v1(mesh_ptr->vertices[indexV0]);
	tzPoint3D v2(mesh_ptr->vertices[indexV1]);
	tzPoint3D v3(mesh_ptr->vertices[indexV2]);
	
	return(tzBBox(fmin(fmin(v1.x, v2.x), v3.x) - delta, fmax(fmax(v1.x, v2.x), v3.x) + delta, 
				fmin(fmin(v1.y, v2.y), v3.y) - delta, fmax(fmax(v1.y, v2.y), v3.y) + delta, 
				fmin(fmin(v1.z, v2.z), v3.z) - delta, fmax(fmax(v1.z, v2.z), v3.z) + delta));
}


//===================================================================================
bool tzMeshTriangle::shadowHit(const tzRay& ray, float& tmin) const 
{
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
	return( (1 - beta - gamma) * mesh_ptr->u[indexUV0] 
				+ beta * mesh_ptr->u[indexUV1]
					+ gamma * mesh_ptr->u[indexUV2] );
}


//===================================================================================
float tzMeshTriangle::interpolate_v(const float beta, const float gamma) const 
{
	return( (1 - beta - gamma) * mesh_ptr->v[indexUV0]
				+ beta * mesh_ptr->v[indexUV1]
					+ gamma * mesh_ptr->v[indexUV2] );
}


