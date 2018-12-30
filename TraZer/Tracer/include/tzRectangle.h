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

		virtual													
		~tzRectangle(void);

		tzRectangle&
		operator= (const tzRectangle& rhs);
		
		tzBBox
		get_bounding_box(void);				
	
		virtual bool 												 
		hit(const tzRay& ray, double& t, tzShadeRec& s) const;	
				
		
		// the following functions are used when the rectangle is a light source
		
		virtual void 								
		set_sampler(tzISampler* sampler); 
				
		virtual tzPoint3D
		sample(void);
		
		virtual tzNormal
		get_normal(const tzPoint3D& p);
		
		virtual float												
		pdf(tzShadeRec& sr);
		
	private:
	
		tzPoint3D 		p0;   			// corner vertex 
		tzVector3D		a;				// side
		tzVector3D		b;				// side
		double			a_len_squared;	// square of the length of side a
		double			b_len_squared;	// square of the length of side b
		tzNormal		normal;
		
		float			area;			// for rectangular lights
		float			inv_area;		// for rectangular lights
		tzISampler*		sampler_ptr;	// for rectangular lights 	
		
		static const double kEpsilon;   											
};

#endif
