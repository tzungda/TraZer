#ifndef TZ_TRACER_FLAT_UV_MESH_TRIANGLE
#define TZ_TRACER_FLAT_UV_MESH_TRIANGLE

#include "../include/tzFlatMeshTriangle.h"

class tzFlatUVMeshTriangle: public tzFlatMeshTriangle {
	public:
		
		tzFlatUVMeshTriangle(void);
		
		tzFlatUVMeshTriangle(tzMesh* _meshPtr, const int i0, const int i1, const int i2);

		virtual tzFlatUVMeshTriangle* clone(void) const;
	
		tzFlatUVMeshTriangle(const tzFlatUVMeshTriangle& fmt);

		virtual
		~tzFlatUVMeshTriangle(void);

		tzFlatUVMeshTriangle&
		operator= (const tzFlatUVMeshTriangle& rhs);
				
		virtual	bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const;

};

#endif

