#ifndef TZ_TRACER_SMOOTH_TRIANGLE
#define TZ_TRACER_SMOOTH_TRIANGLE

#include "../include/tzIGeometricObject.h"

class tzSmoothTriangle: public tzIGeometricObject {
	public:
		
		tzNormal n0, n1, n2;		// normals at each vertex
		
		tzSmoothTriangle(void);
		
		tzSmoothTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c);

		virtual tzSmoothTriangle*
		clone(void) const;
	
		tzSmoothTriangle(const tzSmoothTriangle& st);

		virtual
		~tzSmoothTriangle(void);

		tzSmoothTriangle&
		operator= (const tzSmoothTriangle& rhs);
		
		virtual tzBBox
		get_bounding_box(void);
		
		virtual bool 
		hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const;
		
		virtual bool shadowHit(const tzRay& ray, float& tmin) const;
		
	private:
		
		tzPoint3D v0, v1, v2;	
		
		tzNormal												
		interpolate_normal(const float beta, const float gamma) const;
};

#endif

