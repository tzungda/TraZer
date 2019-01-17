
#ifndef TZ_TRACER_LAMBERTIAN
#define TZ_TRACER_LAMBERTIAN


// This implements a perfect diffuse scatterer

#include "../include/tzIBRDF.h"

class tzLambertian: public tzIBRDF
{
	public:
	
		tzLambertian(void);
		
		tzLambertian(const tzLambertian& lamb);
		
		tzLambertian&
		operator= (const tzLambertian& rhs);
		
		virtual ~tzLambertian(void);
		
		virtual tzLambertian*
		clone(void) const;
		
		virtual tzColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor
		sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
			
		void
		setKa(const float ka);	
				
		void
		setKd(const float kd);
		
		void
		setCd(const tzColor& c);
		
		void													
		setCd(const float r, const float g, const float b);
		
		void													
		setCd(const float c);
					
	private:
	
		float		kd;
		tzColor 	cd;
};




// -------------------------------------------------------------- setKa

inline void
tzLambertian::setKa(const float k) {
	kd = k;
}



// -------------------------------------------------------------- setKd

inline void
tzLambertian::setKd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- setCd

inline void
tzLambertian::setCd(const tzColor& c) {
	cd = c;
}


// ---------------------------------------------------------------- setCd

inline void													
tzLambertian::setCd(const float r, const float g, const float b) {
	cd.r = r; cd.g = g; cd.b = b;
}


// ---------------------------------------------------------------- setCd

inline void													
tzLambertian::setCd(const float c) {
	cd.r = c; cd.g = c; cd.b = c;
}

#endif

