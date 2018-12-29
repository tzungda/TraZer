#ifndef TZ_TRACER_RECTANGLE
#define TZ_TRACER_RECTANGLE

#include "../include/tzISampler.h"
#include "../include/tzIGeometricObject.h"

class tzRectangle: public tzIGeometricObject {
	public:
		
		tzRectangle(void);
				
		tzRectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
		
		tzRectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n);
		
		virtual tzRectangle*
		clone(void) const;
	
		tzRectangle(const tzRectangle& r);

		virtual													
		~tzRectangle(void);

		tzRectangle&
		operator= (const tzRectangle& rhs);
		
		BBox
		get_bounding_box(void);				
	
		virtual bool 												 
		hit(const tzRay& ray, double& t, tzShadeRec& s) const;	
				
		
		// the following functions are used when the rectangle is a light source
		
		virtual void 								
		set_sampler(tzISampler* sampler); 
				
		virtual Point3D 											
		sample(void);
		
		virtual Normal 																
		get_normal(const Point3D& p);		
		
		virtual float												
		pdf(tzShadeRec& sr);
		
	private:
	
		Point3D 		p0;   			// corner vertex 
		Vector3D		a;				// side
		Vector3D		b;				// side
		double			a_len_squared;	// square of the length of side a
		double			b_len_squared;	// square of the length of side b
		Normal			normal;	
		
		float			area;			// for rectangular lights
		float			inv_area;		// for rectangular lights
		tzISampler*		sampler_ptr;	// for rectangular lights 	
		
		static const double kEpsilon;   											
};

#endif
