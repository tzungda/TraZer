#ifndef TZ_TRACER_AMBIENT
#define TZ_TRACER_AMBIENT


#include "../include/tzILight.h"

class tzAmbient: public tzILight {
	public:
	
		tzAmbient(void);

		tzAmbient(const tzAmbient& a);
	
		virtual tzILight*
		clone(void) const;	
		
		tzAmbient&
		operator= (const tzAmbient& rhs);
		
		virtual ~tzAmbient(void);
				
		void
		scale_radiance(const float b);
		
		void
		set_color(const float c);
		
		void
		set_color(const RGBColor& c);
		
		void
		set_color(const float r, const float g, const float b); 
		
		virtual Vector3D								
		get_direction(tzShadeRec& s); 
		
		virtual RGBColor
		L(tzShadeRec& s);
	
	private:
	
		float		ls;
		RGBColor	color;
};




// ------------------------------------------------------------------------------- scale_radiance

inline void
tzAmbient::scale_radiance(const float b) {
	ls = b;
}

// ------------------------------------------------------------------------------- set_color

inline void
tzAmbient::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzAmbient::set_color(const RGBColor& c) {
	color = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzAmbient::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


#endif
