#ifndef TZ_TRACER_PERFECT_SPECULAR
#define TZ_TRACER_PERFECT_SPECULAR

#include "../include/tzIBRDF.h"
#include "tzNormal.h"

class tzPerfectSpecular: public tzIBRDF
{
	public:
	
		tzPerfectSpecular(void);
		
		~tzPerfectSpecular(void);
		
		virtual tzPerfectSpecular*
		clone(void) const;
		
		void
		set_kr(const float k);
		
		void
		set_cr(const tzRGBColor& c);
		
		void													
		set_cr(const float r, const float g, const float b);
		
		void													
		set_cr(const float c);
		
		virtual tzRGBColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const;
		
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzRGBColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
				
	private:
	
		float		kr;			// reflection coefficient
		tzRGBColor 	cr;			// the reflection colour
};


// -------------------------------------------------------------- set_kr

inline void
tzPerfectSpecular::set_kr(const float k) {
	kr = k;
}


// -------------------------------------------------------------- set_cr

inline void
tzPerfectSpecular::set_cr(const tzRGBColor& c) {
	cr = c;
}


// ---------------------------------------------------------------- set_cr

inline void													
tzPerfectSpecular::set_cr(const float r, const float g, const float b) {
	cr.r = r; cr.g = g; cr.b = b;
}


// ---------------------------------------------------------------- set_cr

inline void													
tzPerfectSpecular::set_cr(const float c) {
	cr.r = c; cr.g = c; cr.b = c;
}

#endif

