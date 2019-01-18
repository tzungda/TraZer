
#include "../include/tzMatteSV.h"

// ---------------------------------------------------------------- default constructor

tzMatteSV::tzMatteSV(void)
	:	tzIMaterial(),
		mAmbientBRDF(new tzLambertianSV),
		mDiffuseBRDF(new tzLambertianSV)
{}



// ---------------------------------------------------------------- copy constructor

tzMatteSV::tzMatteSV(const tzMatteSV& m)
	: 	tzIMaterial(m)
{
	if(m.mAmbientBRDF)
		mAmbientBRDF = m.mAmbientBRDF->clone(); 
	else  mAmbientBRDF = NULL;
	
	if(m.mDiffuseBRDF)
		mDiffuseBRDF = m.mDiffuseBRDF->clone(); 
	else  mDiffuseBRDF = NULL;
}


// ---------------------------------------------------------------- clone

tzIMaterial*										
tzMatteSV::clone(void) const {
	return (new tzMatteSV(*this));
}	


// ---------------------------------------------------------------- assignment operator

tzMatteSV&
tzMatteSV::operator= (const tzMatteSV& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzIMaterial::operator=(rhs);
	
	if (mAmbientBRDF) {
		delete mAmbientBRDF;
		mAmbientBRDF = NULL;
	}

	if (rhs.mAmbientBRDF)
		mAmbientBRDF = rhs.mAmbientBRDF->clone();
		
	if (mDiffuseBRDF) {
		delete mDiffuseBRDF;
		mDiffuseBRDF = NULL;
	}

	if (rhs.mDiffuseBRDF)
		mDiffuseBRDF = rhs.mDiffuseBRDF->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzMatteSV::~tzMatteSV(void) {

	if (mAmbientBRDF) {
		delete mAmbientBRDF;
		mAmbientBRDF = NULL;
	}
	
	if (mDiffuseBRDF) {
		delete mDiffuseBRDF;
		mDiffuseBRDF = NULL;
	}
}


// ---------------------------------------------------------------- shade

tzColor
tzMatteSV::shade(tzShadeRec& sr) 
{
	tzVector3D 	wo 			= -sr.mRay.mDirection;
	tzColor 	L 			= mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights	= (int)sr.mWorld.mLights.size();
	
	for (int j = 0; j < num_lights; j++) 
	{
		tzILight* light_ptr = sr.mWorld.mLights[j];
		tzVector3D wi = light_ptr->getDirection( sr );
		wi.normalize();
		float ndotwi = (float)(sr.mNormal * wi);
		float ndotwo = (float)(sr.mNormal * wo);
	
		if (ndotwi > 0.0 && ndotwo)
		{
			// check if it's in shadow
			bool inShadow = false;
			if ( sr.mWorld.mLights[j]->castsShadow() )
			{
				tzRay shadowRay( sr.mHitPoint, wi );
				inShadow = sr.mWorld.mLights[j]->inShadow( shadowRay, sr );
			}

			if ( !inShadow )
			{
				L += mDiffuseBRDF->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi;
			}
		}
	}
	
	return (L);
}

//===================================================================================
tzColor tzMatteSV::areaLightShade( tzShadeRec &sr) const
{
	tzVector3D 	wo = -sr.mRay.mDirection;
	tzColor 	L = mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights = (int)sr.mWorld.mLights.size();

	for (int j = 0; j < num_lights; j++) {
		tzVector3D wi = sr.mWorld.mLights[j]->getDirection(sr);
		float ndotwi = (float)(sr.mNormal * wi);

		if (ndotwi > 0.0)
		{
			// check if it's in shadow
			bool inShadow = false;
			if (sr.mWorld.mLights[j]->castsShadow())
			{
				tzRay shadowRay(sr.mHitPoint, wi);
				inShadow = sr.mWorld.mLights[j]->inShadow(shadowRay, sr);
			}

			if (!inShadow)
			{
				L += mDiffuseBRDF->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
			}
		}
	}

	return (L);
}

//===================================================================================
tzColor tzMatteSV::pathShade(tzShadeRec &sr)
{
	tzVector3D 	wo = -sr.mRay.mDirection;
	tzVector3D 	wi;
	float 		pdf;
	tzColor 	f = mDiffuseBRDF->sampleF(sr, wo, wi, pdf);
	float 		ndotwi = (float)(sr.mNormal * wi);
	tzRay 		reflected_ray(sr.mHitPoint, wi);

	return (f * sr.mWorld.mTracerPtr->traceRay(reflected_ray, sr.mDepth + 1) * ndotwi / pdf);
}
