#ifndef TZ_TRACER_SMOOTH_UV_MESH_TRIANGLE
#define TZ_TRACER_SMOOTH_UV_MESH_TRIANGLE

#include "../include/tzSmoothMeshTriangle.h"

class tzSmoothUVMeshTriangle: public tzSmoothMeshTriangle {
	public:
		
		tzSmoothUVMeshTriangle(void);
		
		tzSmoothUVMeshTriangle(std::shared_ptr<tzMesh> _meshPtr, const int i0, const int i1, const int i2);

		virtual std::shared_ptr<tzIGeometricObject> clone(void) const;
	
		tzSmoothUVMeshTriangle(const tzSmoothUVMeshTriangle& fmt);

		virtual
		~tzSmoothUVMeshTriangle(void);

		tzSmoothUVMeshTriangle&
		operator= (const tzSmoothUVMeshTriangle& rhs);
				
		virtual	bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) ;

};

#endif

