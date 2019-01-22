
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
		
		virtual tzColor f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzColor rho(const tzShadeRec& sr, const tzVector3D& wo) const;
			
		void setKa(const float ka);	
				
		void setKd(const float kd);
		
		void setCd(const tzITexture* c);
					
	private:
	
		float		mKd;
		tzITexture 	*mCd;
};




// -------------------------------------------------------------- setKa

inline void
tzLambertianSV::setKa(const float k) {
	mKd = k;
}



// -------------------------------------------------------------- setmKd

inline void
tzLambertianSV::setKd(const float k) {
	mKd = k;
}


// -------------------------------------------------------------- setCd

inline void
tzLambertianSV::setCd(const tzITexture* c) {
	mCd = (tzITexture*)c;
}


#endif

