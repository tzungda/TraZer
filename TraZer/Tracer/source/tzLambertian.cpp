
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

tzRGBColor
tzLambertian::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (kd * cd * invPI);
}


// ---------------------------------------------------------------------- sample_f

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in path_shade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution

tzRGBColor
tzLambertian::sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const {
	
	tzVector3D w = sr.mNormal;
	tzVector3D v = tzVector3D(0.0034, 1, 0.0071) ^ w;
	v.normalize();
	tzVector3D u = v ^ w;
	
	tzPoint3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize(); 	
	
	pdf = sr.mNormal * wi * invPI;
	
	return (kd * cd * invPI); 
}



// ---------------------------------------------------------------------- rho

tzRGBColor
tzLambertian::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (kd * cd);
}


