
#include "../include/tzMatte.h"

// ---------------------------------------------------------------- default constructor

tzMatte::tzMatte(void)
	:	tzIMaterial(),
		mAmbientBRDF(new tzLambertian),
		mDiffuseBRDF(new tzLambertian)
{}



// ---------------------------------------------------------------- copy constructor

tzMatte::tzMatte(const tzMatte& m)
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
tzMatte::clone(void) const {
	return (new tzMatte(*this));
}	


// ---------------------------------------------------------------- assignment operator

tzMatte&
tzMatte::operator= (const tzMatte& rhs) {
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

tzMatte::~tzMatte(void) {

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

tzColor tzMatte::shade(tzShadeRec& sr) 
{
	tzVector3D 	wo 			= -sr.mRay.d;
	tzColor 	L 			= mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights	= (int)sr.mWorld.mLights.size();
	
	for (int j = 0; j < num_lights; j++) 
	{
		tzVector3D wi = sr.mWorld.mLights[j]->getDirection(sr);
		float ndotwi = (float)(sr.mNormal * wi);
	
		if (ndotwi > 0.0) 
		{
			// check if it's in shadow
			bool inShadow = false;
			if ( sr.mWorld.mLights[j]->castsShadow() )
			{
				tzRay shadowRay( sr.mHitPoint, wi );
				inShadow = sr.mWorld.mLights[j]->inShadow( shadowRay, sr );
			}

			if ( !inShadow)
			{
				L += mDiffuseBRDF->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * ndotwi;
			}
		}
	}
	
	return (L);
}

//===================================================================================
tzColor tzMatte::areaLightShade( tzShadeRec &sr) const
{
	tzVector3D 	wo = -sr.mRay.d;
	tzColor 	L = mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights = (int)sr.mWorld.mLights.size();

	for (int j = 0; j < num_lights; j++) 
	{
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
tzColor tzMatte::pathShade(tzShadeRec &sr)
{
	tzVector3D 	wo = -sr.mRay.d;
	tzVector3D 	wi;
	float 		pdf;
	tzColor 	f = mDiffuseBRDF->sampleF(sr, wo, wi, pdf);
	float 		ndotwi = sr.mNormal * wi;
	tzRay 		reflected_ray(sr.mHitPoint, wi);

	return (f * sr.mWorld.mTracerPtr->traceRay(reflected_ray, sr.mDepth + 1) * ndotwi / pdf);
}
