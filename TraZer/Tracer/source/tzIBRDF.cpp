
#include "../include/tzConstants.h"
#include "../include/tzIBRDF.h"

// ---------------------------------------------------------- default constructor

tzIBRDF::tzIBRDF(void)
	: mSamplerPtr(NULL)
{}


// ---------------------------------------------------------- copy constructor

tzIBRDF::tzIBRDF(const tzIBRDF& brdf) {
	if(brdf.mSamplerPtr)
		mSamplerPtr	= brdf.mSamplerPtr->clone(); 
	else  mSamplerPtr = NULL;
}	



// --------------------------------------------------------------- assignment operator

tzIBRDF&
tzIBRDF::operator= (const tzIBRDF& rhs) {
	if (this == &rhs)
		return (*this);
		
	if (mSamplerPtr) {
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}

	if (rhs.mSamplerPtr)
		mSamplerPtr	= rhs.mSamplerPtr->clone();

	return (*this);
}


// ---------------------------------------------------------- destructor

tzIBRDF::~tzIBRDF(void) {
	if (mSamplerPtr) {
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}
}  



// ---------------------------------------------------------- setSampler

void
tzIBRDF::setSampler(tzISampler* sPtr) {
	mSamplerPtr = sPtr;
	mSamplerPtr->mapSamplesToHemisphere(1);  // for perfect diffuse
}


// ------------------------------------------------------------------------ f

tzColor
tzIBRDF::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------------ sampleF

tzColor
tzIBRDF::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------------ sampleF

tzColor
tzIBRDF::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const {
	return (black);
}


// ------------------------------------------------------------------------ rho	
	
tzColor
tzIBRDF::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (black);
}

