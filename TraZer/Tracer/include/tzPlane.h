#ifndef TZ_TRACER_PLANE
#define TZ_TRACER_PLANE

#include "../include/tzIGeometricObject.h"

//-------------------------------------------------------------------- class Plane

class tzPlane: public tzIGeometricObject {
	
	public:
	
		tzPlane(void);   												// default constructor
		
		tzPlane(const Point3D& point, const Normal& normal);			// constructor	
	
		tzPlane(const tzPlane& plane); 									// copy constructor
		
		virtual tzPlane* 												// virtual copy constructor
		clone(void) const;

		tzPlane& 														// assignment operator
		operator= (const tzPlane& rhs);
		
		virtual ~tzPlane(void);
					
		//
		virtual bool shadow_hit(const tzRay &ray, float &tmin) const;
		//
		virtual bool hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const;

		
		
	private:
	
		Point3D 	a;   				// point through which plane passes 
		Normal 		n;					// normal to the plane
				
		static const double kEpsilon;   // for shadows and secondary rays
};

#endif
