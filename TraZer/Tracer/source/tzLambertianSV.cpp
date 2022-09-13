
#include "../include/tzLambertianSV.h"
#include "../include/tzConstants.h"

//===================================================================================
tzLambertianSV::tzLambertianSV(void)
	: tzIBRDF(),
		mKd(0.0), 
		mCd(NULL)
{}

//===================================================================================
tzLambertianSV::tzLambertianSV(const tzLambertianSV& lamb)
	: tzIBRDF(lamb),
		mKd(lamb.mKd), 
		mCd(lamb.mCd)
{}

//===================================================================================
tzLambertianSV& tzLambertianSV::operator= (const tzLambertianSV& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzIBRDF::operator= (rhs);
	
	mKd = rhs.mKd; 
	mCd = rhs.mCd;
	
	return (*this);
}

//===================================================================================
tzLambertianSV::~tzLambertianSV(void) 
{}

//===================================================================================
std::shared_ptr<tzIBRDF> tzLambertianSV::clone(void) const
{
	return (std::make_shared< tzLambertianSV >(*this));
}	

//===================================================================================
tzColor tzLambertianSV::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const 
{
	return (mKd * mCd->getColor(sr) * (float)invPI);
}

//===================================================================================
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

//===================================================================================
tzColor tzLambertianSV::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return ( mKd * mCd->getColor(sr));
}


