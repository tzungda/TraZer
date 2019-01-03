#ifndef TZ_TRACE_TRIANGLE
#define TZ_TRACE_TRIANGLE



#include "../include/tzIGeometricObject.h"

class tzTriangle: public tzIGeometricObject {
	public:
	
		tzTriangle(void);
				
		tzTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c);

		virtual tzTriangle*
		clone(void) const;
	
		tzTriangle(const tzTriangle& triangle);

		~tzTriangle(void);

		tzTriangle&
		operator= (const tzTriangle& rhs);
		
		virtual tzBBox															
		get_bounding_box(void);

		void 
		compute_normal(void);
		
		virtual bool 
		hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const;		 					 
		
		virtual bool shadowHit(const tzRay& ray, double& tmin) const;
		
	private:
	
		tzPoint3D	v0, v1, v2;
		tzNormal	normal;
};

#endif

