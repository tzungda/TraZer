#ifndef TZ_TRACER_SMOOTH_TRIANGLE
#define TZ_TRACER_SMOOTH_TRIANGLE

#include "../include/tzIGeometricObject.h"

class tzSmoothTriangle: public tzIGeometricObject {
	public:
		
		tzNormal n0, n1, n2;		// normals at each vertex
		
		tzSmoothTriangle(void);
		
		tzSmoothTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c);

		virtual std::shared_ptr<tzIGeometricObject>  clone(void) const;
	
		tzSmoothTriangle(const tzSmoothTriangle& st);

		virtual
		~tzSmoothTriangle(void);

		tzSmoothTriangle&
		operator= (const tzSmoothTriangle& rhs);
		
		virtual tzBBox getBoundingBox() const;
		
		virtual bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) ;
		
		virtual bool shadowHit(const tzRay& ray, const tzShadeRec& sr, float& tmin) const;
		
	private:
		
		tzPoint3D v0, v1, v2;	
		
		tzNormal												
		interpolateNormal(const float beta, const float gamma) const;
};

#endif

