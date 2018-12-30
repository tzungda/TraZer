#ifndef TZ_TRACER_PLANE
#define TZ_TRACER_PLANE

#include "../include/tzIGeometricObject.h"

//-------------------------------------------------------------------- class Plane

class tzPlane: public tzIGeometricObject {
	
	public:
	
		tzPlane(void);   												// default constructor
		
		tzPlane(const tzPoint3D& point, const tzNormal& normal);		// constructor	
	
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
	
		tzPoint3D 	a;   				// point through which plane passes 
		tzNormal	n;					// normal to the plane
				
		static const double kEpsilon;   // for shadows and secondary rays
};

#endif
