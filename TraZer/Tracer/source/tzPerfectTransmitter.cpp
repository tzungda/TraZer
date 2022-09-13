
#include "../include/tzPerfectTransmitter.h"

//===================================================================================
tzPerfectTransmitter::tzPerfectTransmitter(void)
	: 	tzIBTDF(),
		mKt(0.0), 
		mIor(1.0)
{}

//===================================================================================
tzPerfectTransmitter::tzPerfectTransmitter(const tzPerfectTransmitter& pt)
	: 	tzIBTDF(pt),
		mKt(pt.mKt), 
		mIor(pt.mIor)
{}

//===================================================================================
std::shared_ptr<tzIBTDF> tzPerfectTransmitter::clone(void) 
{
	return (std::make_shared< tzPerfectTransmitter >(*this));
}

//===================================================================================
tzPerfectTransmitter::~tzPerfectTransmitter(void) 
{}

//===================================================================================
tzPerfectTransmitter& tzPerfectTransmitter::operator= (const tzPerfectTransmitter& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	mKt = rhs.mKt;
	mIor = rhs.mIor;

	return (*this);
}

//===================================================================================
bool tzPerfectTransmitter::tir(const tzShadeRec& sr) const 
{
	tzVector3D wo(-sr.mRay.mDirection);
	float cos_thetai = sr.mNormal * wo;  
	float eta = mIor;
	
	if (cos_thetai < 0.0f) 
		eta = 1.0f / eta; 
		
	return (1.0f - (1.0f - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}	

//===================================================================================
tzColor tzPerfectTransmitter::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const
{
	return (black);
}

//===================================================================================
tzColor tzPerfectTransmitter::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wt) const 
{
	tzNormal n(sr.mNormal);
	float cos_thetai = n * wo;
	float eta = mIor;	
		
	if (cos_thetai < 0.0f) {			// transmitted ray is outside     
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0f / eta; 			// invert ior 
	}

	float temp = 1.0f - (1.0f - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrtf(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;   
	
	return ( mKt / (eta * eta) * white / fabsf(sr.mNormal * wt));
}

//===================================================================================
tzColor tzPerfectTransmitter::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return (black);
}


