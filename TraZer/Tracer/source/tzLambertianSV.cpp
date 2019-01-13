
#include "../include/tzLambertianSV.h"
#include "../include/tzConstants.h"

// ---------------------------------------------------------------------- default constructor

tzLambertianSV::tzLambertianSV(void)
	: tzIBRDF(),
		kd(0.0), 
		cd(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

tzLambertianSV::tzLambertianSV(const tzLambertianSV& lamb)
	: tzIBRDF(lamb),
		kd(lamb.kd), 
		cd(lamb.cd)
{}


// ---------------------------------------------------------------------- assignment operator

tzLambertianSV&
tzLambertianSV::operator= (const tzLambertianSV& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzIBRDF::operator= (rhs);
	
	kd = rhs.kd; 
	cd = rhs.cd;
	
	return (*this);
}


// ---------------------------------------------------------------------- destructor

tzLambertianSV::~tzLambertianSV(void) {}


// ---------------------------------------------------------------------- clone

tzLambertianSV*
tzLambertianSV::clone(void) const {
	return (new tzLambertianSV(*this));
}	


// ---------------------------------------------------------------------- f
// there is no sampling here

tzRGBColor
tzLambertianSV::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (kd * cd->getColor(sr) * (float)invPI);
}


// ---------------------------------------------------------------------- sample_f

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in pathShade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution

tzRGBColor
tzLambertianSV::sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const {
	
	tzVector3D w = sr.mNormal;
	tzVector3D v = tzVector3D(0.0034f, 1.0f, 0.0071f) ^ w;
	v.normalize();
	tzVector3D u = v ^ w;
	
	tzPoint3D sp = sampler_ptr->sampleHemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize(); 	
	
	pdf = (float)(sr.mNormal * wi * invPI);
	
	return (kd * cd->getColor(sr) * (float)invPI);
}



// ---------------------------------------------------------------------- rho

tzRGBColor
tzLambertianSV::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (kd * cd->getColor(sr));
}


