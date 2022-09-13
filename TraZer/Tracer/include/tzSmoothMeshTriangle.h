#ifndef TZ_TRACER_SMOOTH_MESH_TRIANGLE
#define TZ_TRACER_SMOOTH_MESH_TRIANGLE

#include "../include/tzMeshTriangle.h"

class tzSmoothMeshTriangle: public tzMeshTriangle {
	public:
		
		tzSmoothMeshTriangle(void);
		
		tzSmoothMeshTriangle(std::shared_ptr<tzMesh> _meshPtr, const int i0, const int i1, const int i2);

		virtual std::shared_ptr<tzIGeometricObject> clone(void) const;
	
		tzSmoothMeshTriangle(const tzSmoothMeshTriangle& fmt);

		virtual ~tzSmoothMeshTriangle(void);

		tzSmoothMeshTriangle& operator= (const tzSmoothMeshTriangle& rhs);
				
		virtual	bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) ;

	protected:
	
		tzNormal interpolateNormal(const float beta, const float gamma) const;
};

#endif

