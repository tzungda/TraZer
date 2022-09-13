
#include "../include/tzPerfectSpecular.h"

//===================================================================================
tzPerfectSpecular::tzPerfectSpecular(void)
	: 	tzIBRDF(),
		mKr(0.0), 
		mCr(1.0)
{}

//===================================================================================
tzPerfectSpecular::~tzPerfectSpecular(void) 
{
}

//===================================================================================
std::shared_ptr<tzIBRDF> tzPerfectSpecular::clone(void) const
{
	return (std::make_shared<tzPerfectSpecular>(*this));
}	

//===================================================================================
tzColor tzPerfectSpecular::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const 
{
	return (black);
}

//===================================================================================
tzColor tzPerfectSpecular::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const 
{
	float ndotwo = sr.mNormal * wo;
	wi = -wo + 2.0 * sr.mNormal * ndotwo; 
	return ( mKr * mCr / fabs(sr.mNormal * wi)); // why is this fabs? // kr would be a Fresnel term in a Fresnel reflector
}											 // for transparency when ray hits inside surface?, if so it should go in Chapter 24

//===================================================================================
tzColor tzPerfectSpecular::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const 
{
	float ndotwo = sr.mNormal * wo;
	wi = -wo + 2.0 * sr.mNormal * ndotwo; 
	pdf = fabs(sr.mNormal * wi);
	return ( mKr * mCr);  
}

//===================================================================================
tzColor tzPerfectSpecular::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return (black);
}

