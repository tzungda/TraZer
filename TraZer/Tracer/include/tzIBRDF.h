#ifndef TZ_TRACER_BRDF
#define TZ_TRACER_BRDF


#include "tzRGBColor.h"
#include "tzVector3D.h"
#include "../include/tzShadeRec.h"
#include "../include/tzISampler.h"

class tzIBRDF {
	public:
	
		tzIBRDF(void);
		
		tzIBRDF(const tzIBRDF& brdf);
		
		virtual tzIBRDF*
		clone(void)const = 0;
		
		tzIBRDF&
		operator= (const tzIBRDF& rhs);
		
		virtual ~tzIBRDF(void);
				
		void							
		set_sampler(tzISampler* sPtr);
		
		virtual tzRGBColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const;
		
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzRGBColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
		
			
	protected:
	
		tzISampler* sampler_ptr;		// for indirect illumination
};

#endif
