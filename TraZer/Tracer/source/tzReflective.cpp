
#include "../include/tzReflective.h"

//===================================================================================
tzReflective::tzReflective(void)
	:	tzPhong()
{
	mReflectiveBRDF = std::make_shared<tzPerfectSpecular>();
}


//===================================================================================
tzReflective::tzReflective(const tzReflective& rm)
	: 	tzPhong(rm) 
{
	
	if(rm.mReflectiveBRDF)
		mReflectiveBRDF = std::dynamic_pointer_cast<tzPerfectSpecular>( rm.mReflectiveBRDF->clone() );
	else  
		mReflectiveBRDF = nullptr;
}


//===================================================================================
tzReflective& tzReflective::operator= (const tzReflective& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzPhong::operator=(rhs);


	if (rhs.mReflectiveBRDF)
		mReflectiveBRDF = std::dynamic_pointer_cast<tzPerfectSpecular>( rhs.mReflectiveBRDF->clone() );

	return (*this);
}

//===================================================================================
std::shared_ptr<tzIMaterial> tzReflective::clone(void) const
{
	return (std::make_shared< tzReflective >(*this));
}	

//===================================================================================
tzReflective::~tzReflective(void) 
{
}

//===================================================================================
tzColor tzReflective::shade(tzShadeRec& sr) 
{
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

