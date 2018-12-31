
#ifndef TZ_TRACER_LAMBERTIAN_SV
#define TZ_TRACER_LAMBERTIAN_SV


// This implements a perfect diffuse scatterer

#include "../include/tzIBRDF.h"
#include "../include/tzITexture.h"

class tzLambertianSV: public tzIBRDF
{
	public:
	
		tzLambertianSV(void);
		
		tzLambertianSV(const tzLambertianSV& lamb);
		
		tzLambertianSV&
		operator= (const tzLambertianSV& rhs);
		
		virtual ~tzLambertianSV(void);
		
		virtual tzLambertianSV*
		clone(void) const;
		
		virtual tzRGBColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzRGBColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
			
		void
		set_ka(const float ka);	
				
		void
		set_kd(const float kd);
		
		void
		set_cd(const tzITexture* c);
					
	private:
	
		float		kd;
		tzITexture 	*cd;
};




// -------------------------------------------------------------- set_ka

inline void
tzLambertianSV::set_ka(const float k) {
	kd = k;
}



// -------------------------------------------------------------- set_kd

inline void
tzLambertianSV::set_kd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- set_cd

inline void
tzLambertianSV::set_cd(const tzITexture* c) {
	cd = (tzITexture*)c;
}


#endif

