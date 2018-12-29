#ifndef TZ_TRACER_BRDF
#define TZ_TRACER_BRDF


#include "RGBColor.h"
#include "Vector3D.h"
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
		
		virtual RGBColor
		f(const tzShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
		
		virtual RGBColor
		sample_f(const tzShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
		
		virtual RGBColor
		sample_f(const tzShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
		
		virtual RGBColor
		rho(const tzShadeRec& sr, const Vector3D& wo) const;
		
			
	protected:
	
		tzISampler* sampler_ptr;		// for indirect illumination
};

#endif
