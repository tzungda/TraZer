#ifndef TZ_TRACER_FLAT_MESH_TRIANGLE
#define TZ_TRACER_FLAT_MESH_TRIANGLE

#include "../include/tzMeshTriangle.h"

class tzFlatMeshTriangle: public tzMeshTriangle {	
	public:
		
		tzFlatMeshTriangle(void);
		
		tzFlatMeshTriangle(tzMesh* _meshPtr, const int i0, const int i1, const int i2);

		tzFlatMeshTriangle(tzMesh* _meshPtr, const int v0, const int v1, const int v2, const int n0, const int n1, const int n2, const int uv0, const int uv1, const int uv2);

		virtual tzFlatMeshTriangle* clone(void) const;
	
		tzFlatMeshTriangle(const tzFlatMeshTriangle& fmt);

		virtual ~tzFlatMeshTriangle(void);

		tzFlatMeshTriangle& operator= (const tzFlatMeshTriangle& rhs);
				
		virtual	bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const;

};

#endif

