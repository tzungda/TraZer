#ifndef TZ_TRACER_MESH_TRIANGLE
#define TZ_TRACER_MESH_TRIANGLE


#include "../include/tzIGeometricObject.h"
#include "tzMesh.h"

class tzMeshTriangle: public tzIGeometricObject {	
	public:

		tzMesh* 	mMeshPtr;					// stores all the data
		int			mIndexV0, mIndexV1, mIndexV2;  	// indices into the vertices array in the mesh
		int			mIndexN0, mIndexN1, mIndexN2;  	// indices into the normals array in the mesh
		int			mIndexUV0, mIndexUV1, mIndexUV2;  	// indices into the UVs array in the mesh
		tzNormal	mNormal;					
		float		mArea;						// required for translucency
	
	public:
		
		tzMeshTriangle(void);
		
		tzMeshTriangle(tzMesh* _mesh_ptr, const int i1, const int i2, const int i3);

		tzMeshTriangle(tzMesh* _mesh_ptr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2);

		virtual tzMeshTriangle* clone(void) const = 0;
	
		tzMeshTriangle(const tzMeshTriangle& mt);

		virtual ~tzMeshTriangle(void);

		tzMeshTriangle& operator= (const tzMeshTriangle& rhs);
		
		virtual bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const = 0;
		
		virtual	bool shadowHit(const tzRay& ray, float& tmin) const;
		
		void computeNormal(const bool reverse_normal);
				
		virtual tzNormal getNormal(void) const;				

		virtual tzBBox getBoundingBox(void);
		
	protected:
	
		float interpolate_u(const float beta, const float gamma) const;
		
		float interpolate_v(const float beta, const float gamma) const;			
};

#endif

