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
		setKr(const float k);
		
		void
		setCr(const tzColor& c);
		
		void													
		setCr(const float r, const float g, const float b);
		
		void													
		setCr(const float c);
		
		virtual tzColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor
		sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const;
		
		virtual tzColor
		sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
				
	private:
	
		float		kr;			// reflection coefficient
		tzColor 	cr;			// the reflection colour
};


// -------------------------------------------------------------- setKr

inline void
tzPerfectSpecular::setKr(const float k) {
	kr = k;
}


// -------------------------------------------------------------- setCr

inline void
tzPerfectSpecular::setCr(const tzColor& c) {
	cr = c;
}


// ---------------------------------------------------------------- setCr

inline void													
tzPerfectSpecular::setCr(const float r, const float g, const float b) {
	cr.r = r; cr.g = g; cr.b = b;
}


// ---------------------------------------------------------------- setCr

inline void													
tzPerfectSpecular::setCr(const float c) {
	cr.r = c; cr.g = c; cr.b = c;
}

#endif

