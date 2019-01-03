#ifndef TZ_TRACER_FLAT_MESH_TRIANGLE
#define TZ_TRACER_FLAT_MESH_TRIANGLE

#include "../include/tzMeshTriangle.h"

class tzFlatMeshTriangle: public tzMeshTriangle {	
	public:
		
		tzFlatMeshTriangle(void);
		
		tzFlatMeshTriangle(tzMesh* _meshPtr, const int i0, const int i1, const int i2);

		virtual tzFlatMeshTriangle*
		clone(void) const;
	
		tzFlatMeshTriangle(const tzFlatMeshTriangle& fmt);

		virtual
		~tzFlatMeshTriangle(void);

		tzFlatMeshTriangle&
		operator= (const tzFlatMeshTriangle& rhs);
				
		virtual	bool
		hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const; 																	

};

#endif

