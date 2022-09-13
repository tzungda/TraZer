#ifndef TZ_TRACER_FLAT_UV_MESH_TRIANGLE
#define TZ_TRACER_FLAT_UV_MESH_TRIANGLE

#include "../include/tzFlatMeshTriangle.h"

class tzFlatUVMeshTriangle: public tzFlatMeshTriangle {
	public:
		
		tzFlatUVMeshTriangle(void);
		
		tzFlatUVMeshTriangle( std::shared_ptr< tzMesh > _meshPtr, const int i0, const int i1, const int i2);

		tzFlatUVMeshTriangle(std::shared_ptr< tzMesh > _meshPtr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2);

		virtual std::shared_ptr<tzIGeometricObject> clone(void) const;
	
		tzFlatUVMeshTriangle(const tzFlatUVMeshTriangle& fmt);

		virtual ~tzFlatUVMeshTriangle(void);

		tzFlatUVMeshTriangle& operator= (const tzFlatUVMeshTriangle& rhs);
				
		virtual	bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) ;

		virtual	bool shadowHit(const tzRay& ray, const tzShadeRec& sr, float& tmin) const;

};

#endif

