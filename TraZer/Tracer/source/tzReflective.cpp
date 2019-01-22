

#include "../include/tzReflective.h"

// ---------------------------------------------------------------- default constructor

tzReflective::tzReflective(void)
	:	tzPhong(),
		mReflectiveBRDF(new tzPerfectSpecular)
{}


// ---------------------------------------------------------------- copy constructor

tzReflective::tzReflective(const tzReflective& rm)
	: 	tzPhong(rm) {
	
	if(rm.mReflectiveBRDF)
		mReflectiveBRDF = rm.mReflectiveBRDF->clone(); 
	else  
		mReflectiveBRDF = NULL;
}


// ---------------------------------------------------------------- assignment operator

tzReflective&
tzReflective::operator= (const tzReflective& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzPhong::operator=(rhs);
	
	if (mReflectiveBRDF) {
		delete mReflectiveBRDF;
		mReflectiveBRDF = NULL;
	}

	if (rhs.mReflectiveBRDF)
		mReflectiveBRDF = rhs.mReflectiveBRDF->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

tzReflective*										
tzReflective::clone(void) const {
	return (new tzReflective(*this));
}	


// ---------------------------------------------------------------- destructor

tzReflective::~tzReflective(void) {
	if (mReflectiveBRDF) {
		delete mReflectiveBRDF;
		mReflectiveBRDF = NULL;
	}
}


// ------------------------------------------------------------------------------------ shade 

tzColor
tzReflective::shade(tzShadeRec& sr) {
	tzColor L(tzPhong::shade(sr));  // direct illumination
	
	tzVector3D wo = -sr.mRay.mDirection;
	tzVector3D wi;	
	tzColor fr = mReflectiveBRDF->sampleF(sr, wo, wi); 
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
	tzColor 	fr = mReflectiveBRDF->sampleF(sr, wo, wi, pdf);
	tzRay 		reflected_ray(sr.mHitPoint, wi);

	return (fr * sr.mWorld.mTracerPtr->traceRay(reflected_ray, sr.mDepth + 1) * (sr.mNormal * wi) / pdf);
}

