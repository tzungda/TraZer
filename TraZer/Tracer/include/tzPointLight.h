#ifndef TZ_TRACER_POINTLIGHT
#define TZ_TRACER_POINTLIGHT

#include "../include/tzShadeRec.h"
#include "../include/tzILight.h"


class tzPointLight: public tzILight {
	public:
	
		tzPointLight(void);

		tzPointLight(const tzPointLight& a);
	
		virtual tzILight*
		clone(void) const;	
		
		tzPointLight&
		operator= (const tzPointLight& rhs);
		
		virtual ~tzPointLight(void);
				
		void
		scale_radiance(const float b);
		
		void
		set_color(const float c);
		
		void
		set_color(const tzRGBColor& c);
		
		void
		set_color(const float r, const float g, const float b); 
		
		virtual tzVector3D
		get_direction(tzShadeRec& s); 
		
		virtual tzRGBColor
		L(tzShadeRec& s);

		// 
		void		set_location( const tzVector3D &loc );

		//
		virtual bool inShadow(const tzRay &ray, const tzShadeRec &sr) const;
	
	private:
	
		float		ls;
		tzRGBColor	color;
		tzVector3D	location;
};




// ------------------------------------------------------------------------------- scale_radiance

inline void
tzPointLight::scale_radiance(const float b) {
	ls = b;
}

// ------------------------------------------------------------------------------- set_color

inline void
tzPointLight::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzPointLight::set_color(const tzRGBColor& c) {
	color = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzPointLight::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


#endif

