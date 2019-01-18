

#include "../include/tzReflective.h"

// ---------------------------------------------------------------- default constructor

tzReflective::tzReflective(void)
	:	tzPhong(),
		reflective_brdf(new tzPerfectSpecular)
{}


// ---------------------------------------------------------------- copy constructor

tzReflective::tzReflective(const tzReflective& rm)
	: 	tzPhong(rm) {
	
	if(rm.reflective_brdf)
		reflective_brdf = rm.reflective_brdf->clone(); 
	else  
		reflective_brdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

tzReflective&
tzReflective::operator= (const tzReflective& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzPhong::operator=(rhs);
	
	if (reflective_brdf) {
		delete reflective_brdf;
		reflective_brdf = NULL;
	}

	if (rhs.reflective_brdf)
		reflective_brdf = rhs.reflective_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

tzReflective*										
tzReflective::clone(void) const {
	return (new tzReflective(*this));
}	


// ---------------------------------------------------------------- destructor

tzReflective::~tzReflective(void) {
	if (reflective_brdf) {
		delete reflective_brdf;
		reflective_brdf = NULL;
	}
}


// ------------------------------------------------------------------------------------ shade 

tzColor
tzReflective::shade(tzShadeRec& sr) {
	tzColor L(tzPhong::shade(sr));  // direct illumination
	
	tzVector3D wo = -sr.mRay.mDirection;
	tzVector3D wi;	
	tzColor fr = reflective_brdf->sampleF(sr, wo, wi); 
	tzRay reflected_ray(sr.mHitPoint, wi); 
	reflected_ray.mDepth = sr.mDepth + 1;
	
	L += fr * sr.mWorld.mTracerPtr->traceRay(reflected_ray, sr.mDepth + 1) * (sr.mNormal * wi);
					
	return (L);
}

//===================================================================================
tzColor tzReflective::pathShade(tzShadeRec& sr)
{
	tzVector3D 	wo = -sr.mRay.mDirection;
	tzVector3D 	wi;
	float 		pdf;
	tzColor 	fr = reflective_brdf->sampleF(sr, wo, wi, pdf);
	tzRay 		reflected_ray(sr.mHitPoint, wi);

	return (fr * sr.mWorld.mTracerPtr->traceRay(reflected_ray, sr.mDepth + 1) * (sr.mNormal * wi) / pdf);
}

