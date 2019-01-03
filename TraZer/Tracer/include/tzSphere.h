#ifndef TZ_TRACER_SPHEERE
#define TZ_TRACER_SPHEERE


#include "../include/tzIGeometricObject.h"
#include "../include/tzRay.h"
#include "../include/tzShadeRec.h"

class tzSphere: public tzIGeometricObject {	
								  	
	public:
		
		tzSphere(void);   									// Default constructor
				
		tzSphere(tzPoint3D center, double r);					// Constructor 
					
		tzSphere(const tzSphere& tzSphere); 						// Copy constructor
		
		virtual tzSphere* 									// Virtual copy constructor
		clone(void) const;

		virtual												// Destructor
		~tzSphere(void);   									

		tzSphere& 											// assignment operator
		operator= (const tzSphere& tzSphere);				
																					
		void
		set_center(const tzPoint3D& c);
		
		void
		set_center(const double x, const double y, const double z);
		
		void
		set_radius(const double r);

		virtual bool shadowHit(const tzRay &ray, float &tmin) const;
						
		virtual bool 												 
		hit(const tzRay& ray, double& t, tzShadeRec& s) const;	
		
	private:
	
		tzPoint3D 	center;   			// center coordinates as a point  
		double 		radius;				// the radius 
		
		static const double kEpsilon;   // for shadows and secondary rays
};



inline void
tzSphere::set_center(const tzPoint3D& c) {
	center = c;
}
		
inline void
tzSphere::set_center(const double x, const double y, const double z) {
	center.x = x;
	center.y = y;
	center.z = z;
}
		
inline void
tzSphere::set_radius(const double r) {
	radius = r;
}

#endif

