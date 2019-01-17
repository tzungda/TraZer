#ifndef TZ_TRACER_BRDF
#define TZ_TRACER_BRDF


#include "tzColor.h"
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
		setSampler(tzISampler* sPtr);
		
		virtual tzColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor
		sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const;
		
		virtual tzColor
		sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
		
			
	protected:
	
		tzISampler* sampler_ptr;		// for indirect illumination
};

#endif

