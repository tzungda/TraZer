#ifndef TZ_TRACER_PLANE
#define TZ_TRACER_PLANE

#include "../include/tzIGeometricObject.h"

//-------------------------------------------------------------------- class Plane

class tzPlane: public tzIGeometricObject {
	
	public:
	
		tzPlane(void);   												// default constructor
		
		tzPlane(const tzPoint3D& point, const tzNormal& normal);		// constructor	
	
		tzPlane(const tzPlane& plane); 									// copy constructor
		
		virtual std::shared_ptr<tzIGeometricObject> clone(void) const;

		tzPlane& operator= (const tzPlane& rhs);
		
		virtual ~tzPlane(void);
					
		//
		virtual bool shadowHit(const tzRay &ray, const tzShadeRec& sr, float &tmin) const;
		//
		virtual bool hit(const tzRay& ray, float& tmin, tzShadeRec& sr) ;

		
		
	private:
	
		tzPoint3D 	mPoint;   				// point through which plane passes 
		tzNormal	mNormal;					// normal to the plane
				
		static const float mEpsilon;   // for shadows and secondary rays
};

#endif

