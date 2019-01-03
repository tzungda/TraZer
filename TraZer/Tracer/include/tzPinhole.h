#ifndef TZ_TRACER_PINHOLE
#define TZ_TRACER_PINHOLE


#include "tzPoint2D.h"
#include "../include/tzWorld.h"    // we can #include "World.h" here

//--------------------------------------------------------------------- class Pinhole

class tzPinhole: public tzICamera {
	public:
	
		tzPinhole();

		tzPinhole(const tzPinhole& ph);
		
		virtual tzICamera*						
		clone(void) const;			

		tzPinhole&
		operator= (const tzPinhole& rhs);
		
		virtual ~tzPinhole();
			
		void
		set_view_distance(const float vpd);
		
		void
		set_zoom(const float zoom_factor);
		
		tzVector3D
		get_direction(const tzPoint2D& p) const;
		
		virtual void 												
			renderScene(const tzWorld& w);
		
	private:
			
		float	d;		// view plane distance
		float	zoom;	// zoom factor
		
};




//-------------------------------------------------------------------------- set_vpd

inline void
tzPinhole::set_view_distance(float _d) {
	d = _d;
}	
	


//-------------------------------------------------------------------------- set_zoom

inline void
tzPinhole::set_zoom(float zoom_factor) {
	zoom = zoom_factor;
}	

#endif

