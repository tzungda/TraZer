
#include "../include/tzConstants.h"
#include "../include/tzIBRDF.h"

//===================================================================================
tzIBRDF::tzIBRDF(void)
	: mSamplerPtr(nullptr)
{}

//===================================================================================
tzIBRDF::tzIBRDF(const tzIBRDF& brdf) 
{
	if(brdf.mSamplerPtr)
		mSamplerPtr	= brdf.mSamplerPtr->clone(); 
	else  mSamplerPtr = nullptr;
}	

//===================================================================================
tzIBRDF& tzIBRDF::operator= (const tzIBRDF& rhs) 
{
	if (this == &rhs)
		return (*this);

	if (rhs.mSamplerPtr)
		mSamplerPtr	= rhs.mSamplerPtr->clone();

	return (*this);
}

//===================================================================================
tzIBRDF::~tzIBRDF(void) 
{
}  

//===================================================================================
void tzIBRDF::setSampler(std::shared_ptr< tzISampler > sPtr) 
{
	mSamplerPtr = sPtr;
	mSamplerPtr->mapSamplesToHemisphere(1);  // for perfect diffuse
}

//===================================================================================
tzColor tzIBRDF::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const 
{
	return (black);
}

//===================================================================================
tzColor tzIBRDF::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const 
{
	return (black);
}

//===================================================================================
tzColor tzIBRDF::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const 
{
	return (black);
}

//===================================================================================	
tzColor tzIBRDF::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return (black);
}

