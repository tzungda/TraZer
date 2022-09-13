
#include "../include/tzLambertian.h"
#include "../include/tzConstants.h"

//===================================================================================
tzLambertian::tzLambertian(void)
	:   tzIBRDF(),
		mKd(0.0f), 
		mCd(0.0f)
{}

//===================================================================================
tzLambertian::tzLambertian(const tzLambertian& lamb)
	:   tzIBRDF(lamb),
		mKd(lamb.mKd), 
		mCd(lamb.mCd)
{}

//===================================================================================
tzLambertian& tzLambertian::operator= (const tzLambertian& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzIBRDF::operator= (rhs);
	
	mKd = rhs.mKd; 
	mCd = rhs.mCd;
	
	return (*this);
}

//===================================================================================
tzLambertian::~tzLambertian(void) 
{
}

//===================================================================================
std::shared_ptr<tzIBRDF> tzLambertian::clone(void) const 
{
	return (std::make_shared< tzLambertian >(*this));
}	

//===================================================================================
tzColor tzLambertian::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const 
{
	return (mKd * mCd * (float)invPI);
}

//===================================================================================
tzColor tzLambertian::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const 
{	
	tzVector3D w = sr.mNormal;
	tzVector3D v = tzVector3D(0.0034f, 1.0f, 0.0071f) ^ w;
	v.normalize();
	tzVector3D u = v ^ w;
	
	tzPoint3D sp = mSamplerPtr->sampleHemisphere(sr.mRay);
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize(); 	
	
	pdf = (float)(sr.mNormal * wi * invPI);
	
	return (mKd * mCd * (float)invPI);
}

//===================================================================================
tzColor tzLambertian::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return (mKd * mCd);
}


