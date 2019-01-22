
#include "../include/tzLambertianSV.h"
#include "../include/tzConstants.h"

// ---------------------------------------------------------------------- default constructor

tzLambertianSV::tzLambertianSV(void)
	: tzIBRDF(),
		mKd(0.0), 
		mCd(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

tzLambertianSV::tzLambertianSV(const tzLambertianSV& lamb)
	: tzIBRDF(lamb),
		mKd(lamb.mKd), 
		mCd(lamb.mCd)
{}


// ---------------------------------------------------------------------- assignment operator

tzLambertianSV& tzLambertianSV::operator= (const tzLambertianSV& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzIBRDF::operator= (rhs);
	
	mKd = rhs.mKd; 
	mCd = rhs.mCd;
	
	return (*this);
}


// ---------------------------------------------------------------------- destructor

tzLambertianSV::~tzLambertianSV(void) {}


// ---------------------------------------------------------------------- clone

tzLambertianSV* tzLambertianSV::clone(void) const 
{
	return (new tzLambertianSV(*this));
}	


// ---------------------------------------------------------------------- f
// there is no sampling here

tzColor tzLambertianSV::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const 
{
	return (mKd * mCd->getColor(sr) * (float)invPI);
}


// ---------------------------------------------------------------------- sampleF

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in pathShade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution

tzColor tzLambertianSV::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const 
{
	
	tzVector3D w = sr.mNormal;
	tzVector3D v = tzVector3D(0.0034f, 1.0f, 0.0071f) ^ w;
	v.normalize();
	tzVector3D u = v ^ w;
	
	tzPoint3D sp = mSamplerPtr->sampleHemisphere(sr.mRay);
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize(); 	
	
	pdf = (float)(sr.mNormal * wi * invPI);
	
	return ( mKd * mCd->getColor(sr) * (float)invPI );
}



// ---------------------------------------------------------------------- rho

tzColor tzLambertianSV::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return ( mKd * mCd->getColor(sr));
}


