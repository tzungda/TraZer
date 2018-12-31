
#include "../include/tzPerfectSpecular.h"

// ---------------------------------------------------------- default constructor

tzPerfectSpecular::tzPerfectSpecular(void)
	: 	tzIBRDF(),
		kr(0.0), 
		cr(1.0)
{}

// ---------------------------------------------------------- destructor

tzPerfectSpecular::~tzPerfectSpecular(void) {}


// ---------------------------------------------------------------------- clone

tzPerfectSpecular*
tzPerfectSpecular::clone(void) const {
	return (new tzPerfectSpecular(*this));
}	


// ---------------------------------------------------------- f

tzRGBColor
tzPerfectSpecular::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (black);
}


// ---------------------------------------------------------- sample_f
// this computes wi: the direction of perfect mirror reflection
// it's called from from the functions Reflective::shade and Transparent::shade.
// the fabs in the last statement is for transparency

tzRGBColor
tzPerfectSpecular::sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const {
	float ndotwo = sr.mNormal * wo;
	wi = -wo + 2.0 * sr.mNormal * ndotwo; 
	return (kr * cr / fabs(sr.mNormal * wi)); // why is this fabs? // kr would be a Fresnel term in a Fresnel reflector
}											 // for transparency when ray hits inside surface?, if so it should go in Chapter 24


// ---------------------------------------------------------- sample_f
// this version of sample_f is used with path tracing
// it returns ndotwi in the pdf

tzRGBColor
tzPerfectSpecular::sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const {
	float ndotwo = sr.mNormal * wo;
	wi = -wo + 2.0 * sr.mNormal * ndotwo; 
	pdf = fabs(sr.mNormal * wi);
	return (kr * cr);  
}

// ---------------------------------------------------------- rho

tzRGBColor
tzPerfectSpecular::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (black);
}

