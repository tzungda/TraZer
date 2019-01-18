
#include "../include/tzLambertian.h"
#include "../include/tzConstants.h"

// ---------------------------------------------------------------------- default constructor

tzLambertian::tzLambertian(void)
	:   tzIBRDF(),
		kd(0.0), 
		cd(0.0)
{}


// ---------------------------------------------------------------------- copy constructor

tzLambertian::tzLambertian(const tzLambertian& lamb)
	:   tzIBRDF(lamb),
		kd(lamb.kd), 
		cd(lamb.cd)
{}


// ---------------------------------------------------------------------- assignment operator

tzLambertian&
tzLambertian::operator= (const tzLambertian& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzIBRDF::operator= (rhs);
	
	kd = rhs.kd; 
	cd = rhs.cd;
	
	return (*this);
}


// ---------------------------------------------------------------------- destructor

tzLambertian::~tzLambertian(void) {}


// ---------------------------------------------------------------------- clone

tzLambertian*
tzLambertian::clone(void) const {
	return (new tzLambertian(*this));
}	


// ---------------------------------------------------------------------- f
// there is no sampling here

tzColor
tzLambertian::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (kd * cd * (float)invPI);
}


// ---------------------------------------------------------------------- sampleF

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in pathShade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution

tzColor
tzLambertian::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const {
	
	tzVector3D w = sr.mNormal;
	tzVector3D v = tzVector3D(0.0034f, 1.0f, 0.0071f) ^ w;
	v.normalize();
	tzVector3D u = v ^ w;
	
	tzPoint3D sp = mSamplerPtr->sampleHemisphere(sr.mRay);
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize(); 	
	
	pdf = (float)(sr.mNormal * wi * invPI);
	
	return (kd * cd * (float)invPI);
}



// ---------------------------------------------------------------------- rho

tzColor
tzLambertian::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (kd * cd);
}


