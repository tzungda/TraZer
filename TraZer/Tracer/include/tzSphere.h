#ifndef TZ_TRACER_SPHEERE
#define TZ_TRACER_SPHEERE


#include "../include/tzIGeometricObject.h"
#include "../include/tzRay.h"
#include "../include/tzShadeRec.h"

class tzSphere: public tzIGeometricObject {	
								  	
	public:
		
		tzSphere(void);   									// Default constructor
				
		tzSphere(tzPoint3D center, float r);					// Constructor 
					
		tzSphere(const tzSphere& tzSphere); 						// Copy constructor
		
		virtual tzSphere* clone(void) const;

		virtual ~tzSphere(void);   									

		tzSphere& operator= (const tzSphere& tzSphere);				
																					
		void setCenter(const tzPoint3D& c);
		
		void setCenter(const float x, const float y, const float z);
		
		void setRadius(const float r);

		virtual bool shadowHit(const tzRay &ray, float &tmin) const;
						
		virtual bool hit(const tzRay& ray, float& t, tzShadeRec& s) const;
		
	private:
	
		tzPoint3D 	mCenter;   			// center coordinates as a point  
		float 		mRadius;				// the radius 
		
		static const float mEpsilon;   // for shadows and secondary rays
};



inline void tzSphere::setCenter(const tzPoint3D& c) 
{
	mCenter = c;
}
		
inline void tzSphere::setCenter(const float x, const float y, const float z) 
{
	mCenter.x = x;
	mCenter.y = y;
	mCenter.z = z;
}
		
inline void tzSphere::setRadius(const float r) 
{
	mRadius = r;
}

#endif

