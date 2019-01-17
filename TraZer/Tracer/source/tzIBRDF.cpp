
#include "../include/tzConstants.h"
#include "../include/tzIBRDF.h"

// ---------------------------------------------------------- default constructor

tzIBRDF::tzIBRDF(void)
	: sampler_ptr(NULL)
{}


// ---------------------------------------------------------- copy constructor

tzIBRDF::tzIBRDF(const tzIBRDF& brdf) {
	if(brdf.sampler_ptr)
		sampler_ptr	= brdf.sampler_ptr->clone(); 
	else  sampler_ptr = NULL;
}	



// --------------------------------------------------------------- assignment operator

tzIBRDF&
tzIBRDF::operator= (const tzIBRDF& rhs) {
	if (this == &rhs)
		return (*this);
		
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	if (rhs.sampler_ptr)
		sampler_ptr	= rhs.sampler_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------- destructor

tzIBRDF::~tzIBRDF(void) {
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
}  



// ---------------------------------------------------------- setSampler

void
tzIBRDF::setSampler(tzISampler* sPtr) {
	sampler_ptr = sPtr;
	sampler_ptr->mapSamplesToHemisphere(1);  // for perfect diffuse
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

