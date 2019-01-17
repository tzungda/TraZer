#ifndef TZ_TRACER_RECTANGLE
#define TZ_TRACER_RECTANGLE

#include "../include/tzISampler.h"
#include "../include/tzIGeometricObject.h"

class tzRectangle: public tzIGeometricObject {
	public:
		
		tzRectangle(void);
				
		tzRectangle(const tzPoint3D& _p0, const tzVector3D& _a, const tzVector3D& _b);
		
		tzRectangle(const tzPoint3D& _p0, const tzVector3D& _a, const tzVector3D& _b, const tzNormal& n);
		
		virtual tzRectangle*
		clone(void) const;
	
		tzRectangle(const tzRectangle& r);

		virtual ~tzRectangle(void);

		tzRectangle& operator= (const tzRectangle& rhs);
		
		tzBBox getBoundingBox(void);				
	
		virtual bool hit(const tzRay& ray, float& t, tzShadeRec& s) const;
				
		
		// the following functions are used when the rectangle is a light source
		
		virtual void setSampler(tzISampler* sampler); 
				
		virtual tzPoint3D sample(const tzShadeRec& sr);
		
		virtual tzNormal getNormal(const tzPoint3D& p);
		
		virtual float pdf(tzShadeRec& sr);
		
	private:
	
		tzPoint3D 		p0;   			// corner vertex 
		tzVector3D		a;				// side
		tzVector3D		b;				// side
		float			a_len_squared;	// square of the length of side a
		float			b_len_squared;	// square of the length of side b
		tzNormal		normal;
		
		float			area;			// for rectangular lights
		float			inv_area;		// for rectangular lights
		tzISampler*		sampler_ptr;	// for rectangular lights 	
		
		static const float kEpsilon;
};

#endif
