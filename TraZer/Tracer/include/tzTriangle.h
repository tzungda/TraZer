#ifndef TZ_TRACE_TRIANGLE
#define TZ_TRACE_TRIANGLE



#include "../include/tzIGeometricObject.h"

class tzTriangle: public tzIGeometricObject {
	public:
	
		tzTriangle(void);
				
		tzTriangle(const tzPoint3D& a, const tzPoint3D& b, const tzPoint3D& c);

		virtual tzTriangle* clone(void) const;
	
		tzTriangle(const tzTriangle& triangle);

		~tzTriangle(void);

		tzTriangle& operator= (const tzTriangle& rhs);
		
		virtual tzBBox getBoundingBox(void);

		void  computeNormal(void);
		
		virtual bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const;
		
		virtual bool shadowHit(const tzRay& ray, float& tmin) const;
		
	private:
	
		tzPoint3D	mV0, mV1, mV2;
		tzNormal	mNormal;
};

#endif

