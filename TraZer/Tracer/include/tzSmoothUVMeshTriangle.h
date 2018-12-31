#ifndef TZ_TRACER_SMOOTH_UV_MESH_TRIANGLE
#define TZ_TRACER_SMOOTH_UV_MESH_TRIANGLE

#include "../include/tzSmoothMeshTriangle.h"

class tzSmoothUVMeshTriangle: public tzSmoothMeshTriangle {
	public:
		
		tzSmoothUVMeshTriangle(void);
		
		tzSmoothUVMeshTriangle(tzMesh* _meshPtr, const int i0, const int i1, const int i2);

		virtual tzSmoothUVMeshTriangle*
		clone(void) const;
	
		tzSmoothUVMeshTriangle(const tzSmoothUVMeshTriangle& fmt);

		virtual
		~tzSmoothUVMeshTriangle(void);

		tzSmoothUVMeshTriangle&
		operator= (const tzSmoothUVMeshTriangle& rhs);
				
		virtual	bool 
		hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const; 																	

};

#endif

