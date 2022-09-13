#ifndef TZ_TRACER_PERFECT_SPECULAR
#define TZ_TRACER_PERFECT_SPECULAR

#include "../include/tzIBRDF.h"
#include "tzNormal.h"

class tzPerfectSpecular: public tzIBRDF
{
	public:
	
		tzPerfectSpecular(void);
		
		~tzPerfectSpecular(void);
		
		virtual std::shared_ptr<tzIBRDF> clone(void) const;
		
		void setKr(const float k);
		
		void setCr(const tzColor& c);
		
		void setCr(const float r, const float g, const float b);
		
		void setCr(const float c);
		
		virtual tzColor f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const;
		
		virtual tzColor sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzColor rho(const tzShadeRec& sr, const tzVector3D& wo) const;
				
	private:
	
		float		mKr;			// reflection coefficient
		tzColor 	mCr;			// the reflection colour
};

//===================================================================================
inline void tzPerfectSpecular::setKr(const float k) 
{
	mKr = k;
}

//===================================================================================
inline void tzPerfectSpecular::setCr(const tzColor& c) 
{
	mCr = c;
}

//===================================================================================
inline void tzPerfectSpecular::setCr(const float r, const float g, const float b) 
{
	mCr.r = r; mCr.g = g; mCr.b = b;
}

//===================================================================================
inline void tzPerfectSpecular::setCr(const float c) 
{
	mCr.r = c; mCr.g = c; mCr.b = c;
}

#endif

