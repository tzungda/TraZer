#ifndef TZ_TRACER_DIRECTIONAL
#define TZ_TRACER_DIRECTIONAL

#include "../include/tzILight.h"
#include "Vector3D.h"
#include "RGBColor.h"

#include "../include/tzWorld.h"			// you will need this later on for shadows
#include "tzShadeRec.h"


class tzDirectional: public tzILight {
	public:
	
		tzDirectional(void);

		tzDirectional(const tzDirectional& dl);
		
		virtual tzILight* 									
		clone(void) const;			

		tzDirectional&
		operator= (const tzDirectional& rhs);
			
		virtual ~tzDirectional(void);
				
		void
		scale_radiance(const float b);
		
		void
		set_color(const float c);
		
		void
		set_color(const RGBColor& c);
		
		void
		set_color(const float r, const float g, const float b); 		
			
		void
		set_direction(Vector3D d);						
		
		void
		set_direction(float dx, float dy, float dz);
		
		virtual Vector3D								
		get_direction(tzShadeRec& sr);
				
		virtual RGBColor		
		L(tzShadeRec& sr);	
		
	private:

		float		ls;			
		RGBColor	color;
		Vector3D	dir;		// direction the light comes from
};


// inlined access functions


// ------------------------------------------------------------------------------- scale_radiance

inline void
tzDirectional::scale_radiance(const float b) {
	ls = b;
}

// ------------------------------------------------------------------------------- set_color

inline void
tzDirectional::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzDirectional::set_color(const RGBColor& c) {
	color = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzDirectional::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------------- set_direction

inline void
tzDirectional::set_direction(Vector3D d) {
	dir = d;
	dir.normalize();
}


// ---------------------------------------------------------------------- set_direction 

inline void
tzDirectional::set_direction(float dx, float dy, float dz) {
	dir.x = dx; dir.y = dy; dir.z = dz;
	dir.normalize();
}


#endif

