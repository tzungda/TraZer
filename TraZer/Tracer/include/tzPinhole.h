#ifndef TZ_TRACER_PINHOLE
#define TZ_TRACER_PINHOLE


#include "tzPoint2D.h"
#include "../include/tzWorld.h"    // we can #include "World.h" here


class tzPinhole: public tzICamera {
	public:
	
		tzPinhole();

		tzPinhole(const tzPinhole& ph);
		
		virtual std::shared_ptr<tzICamera> clone(void) const;

		tzPinhole& operator= (const tzPinhole& rhs);
		
		virtual ~tzPinhole();
			
		void setViewDistance(const float vpd);
		
		void setZoom(const float zoomFactor);
		
		tzVector3D getDirection(const tzPoint2D& p) const;
		
		virtual void renderScene(const tzWorld& w) const;
		
	private:
			
		float	mDistance;		// view plane distance
		float	mZoom;	// zoom factor
		
};


//===================================================================================
inline void tzPinhole::setViewDistance(float d) 
{
	mDistance = d;
}	
	
//===================================================================================
inline void tzPinhole::setZoom(float zoomFactor) 
{
	mZoom = zoomFactor;
}	

#endif

