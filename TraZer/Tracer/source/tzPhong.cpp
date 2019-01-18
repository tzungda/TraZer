
#include "../include/tzPhong.h"

// ---------------------------------------------------------------- default constructor

tzPhong::tzPhong(void)
	:	tzIMaterial(),
		mAmbientBRDF(new tzLambertian),
		mDiffuseBRDF(new tzLambertian),
		specular_brdf( new tzGlossySpecular )
{}



// ---------------------------------------------------------------- copy constructor

tzPhong::tzPhong(const tzPhong& m)
	: 	tzIMaterial(m)
{
	if(m.mAmbientBRDF)
		mAmbientBRDF = m.mAmbientBRDF->clone(); 
	else  mAmbientBRDF = NULL;
	
	if(m.mDiffuseBRDF)
		mDiffuseBRDF = m.mDiffuseBRDF->clone(); 
	else  mDiffuseBRDF = NULL;

	//
	if (m.specular_brdf)
	{
		specular_brdf = m.specular_brdf->clone();
	}
	else
	{
		specular_brdf = NULL;
	}
}


// ---------------------------------------------------------------- clone

tzIMaterial*										
tzPhong::clone(void) const {
	return (new tzPhong(*this));
}	


// ---------------------------------------------------------------- assignment operator

tzPhong&
tzPhong::operator= (const tzPhong& rhs) {
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

	//
	if ( specular_brdf )
	{
		delete specular_brdf;
		specular_brdf = NULL;
	}
	if ( rhs.specular_brdf )
	{
		specular_brdf = rhs.specular_brdf->clone();
	}

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzPhong::~tzPhong(void) {

	if (mAmbientBRDF) {
		delete mAmbientBRDF;
		mAmbientBRDF = NULL;
	}
	
	if (mDiffuseBRDF) {
		delete mDiffuseBRDF;
		mDiffuseBRDF = NULL;
	}

	if (specular_brdf )
	{
		delete specular_brdf;
		specular_brdf = NULL;
	}
}


// ---------------------------------------------------------------- shade

tzColor tzPhong::shade(tzShadeRec& sr) 
{
	tzVector3D 	wo 			= -sr.mRay.mDirection;
	tzColor	ambientColor = mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	tzColor 	L 			= ambientColor;
	int 		num_lights	= (int)sr.mWorld.mLights.size();
	
	for (int j = 0; j < num_lights; j++) {
		tzVector3D wi = sr.mWorld.mLights[j]->getDirection(sr);
		float ndotwi = (float)(sr.mNormal * wi);
	
		if (ndotwi > 0.0) 
		{
			// check if it's in shadow
			bool in_shadow = false;
			if ( sr.mWorld.mLights[j]->castsShadow( ) )
			{
				tzRay shadowRay( sr.mHitPoint, wi );
				in_shadow = sr.mWorld.mLights[j]->inShadow( shadowRay, sr );
			}

			//
			if ( !in_shadow )
			{
				// L += mDiffuseBRDF->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
				L += ( mDiffuseBRDF->f(sr, wo, wi) + specular_brdf->f( sr, wo, wi ) ) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
			}
		}
	}
	
	return (L);
}

//===================================================================================
tzColor tzPhong::areaLightShade( tzShadeRec &sr) const
{
	tzVector3D 	wo = -sr.mRay.mDirection;
	tzColor	ambientColor = mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	tzColor 	L = ambientColor;
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

			//
			if (!inShadow)
			{
				L += (mDiffuseBRDF->f(sr, wo, wi) + specular_brdf->f(sr, wo, wi)) * sr.mWorld.mLights[j]->L(sr) * ndotwi;
			}
		}
	}

	return (L);
}


