#ifndef TZ_TRACER_AMBIENTOCCLUDER
#define TZ_TRACER_AMBIENTOCCLUDER

#include "../include/tzISampler.h"
#include "../include/tzILight.h"

class tzAmbientOccluder: public tzILight {
	public:
	
		tzAmbientOccluder(void);

		tzAmbientOccluder(const tzAmbientOccluder& a);
	
		virtual tzILight*
		clone(void) const;	
		
		tzAmbientOccluder&
		operator= (const tzAmbientOccluder& rhs);
		
		virtual ~tzAmbientOccluder(void);
				
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
		virtual bool	inShadow(const tzRay &ray, const tzShadeRec &sr) const;

		//
		void		setSampler( tzISampler *samplerPtr );

		//
		void		setMinAmount( float minAmount );
	
	private:
	
		float		ls;
		tzRGBColor	color;
		tzISampler	*mSamplerPtr;
		tzVector3D	u, v, w;
		tzRGBColor	min_amount;
};




// ------------------------------------------------------------------------------- scale_radiance

inline void
tzAmbientOccluder::scale_radiance(const float b) {
	ls = b;
}

// ------------------------------------------------------------------------------- set_color

inline void
tzAmbientOccluder::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzAmbientOccluder::set_color(const tzRGBColor& c) {
	color = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
tzAmbientOccluder::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


#endif

