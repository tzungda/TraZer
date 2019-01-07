
#include "../include/tzMatte.h"

// ---------------------------------------------------------------- default constructor

tzMatte::tzMatte(void)
	:	tzIMaterial(),
		ambient_brdf(new tzLambertian),
		diffuse_brdf(new tzLambertian)
{}



// ---------------------------------------------------------------- copy constructor

tzMatte::tzMatte(const tzMatte& m)
	: 	tzIMaterial(m)
{
	if(m.ambient_brdf)
		ambient_brdf = m.ambient_brdf->clone(); 
	else  ambient_brdf = NULL;
	
	if(m.diffuse_brdf)
		diffuse_brdf = m.diffuse_brdf->clone(); 
	else  diffuse_brdf = NULL;
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
	
	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}

	if (rhs.ambient_brdf)
		ambient_brdf = rhs.ambient_brdf->clone();
		
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}

	if (rhs.diffuse_brdf)
		diffuse_brdf = rhs.diffuse_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzMatte::~tzMatte(void) {

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}
	
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}
}


// ---------------------------------------------------------------- shade

tzRGBColor tzMatte::shade(tzShadeRec& sr) 
{
	tzVector3D 	wo 			= -sr.mRay.d;
	tzRGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
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
				L += diffuse_brdf->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * ndotwi;
			}
		}
	}
	
	return (L);
}

//===================================================================================
tzRGBColor tzMatte::area_light_shade( tzShadeRec &sr) const
{
	tzVector3D 	wo = -sr.mRay.d;
	tzRGBColor 	L = ambient_brdf->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
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
				L += diffuse_brdf->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
			}
		}
	}

	return (L);
}

//===================================================================================
tzRGBColor tzMatte::path_shade(tzShadeRec &sr)
{
	tzVector3D 	wo = -sr.mRay.d;
	tzVector3D 	wi;
	float 		pdf;
	tzRGBColor 	f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float 		ndotwi = sr.mNormal * wi;
	tzRay 		reflected_ray(sr.mHitPoint, wi);

	return (f * sr.mWorld.mTracerPtr->trace_ray(reflected_ray, sr.mDepth + 1) * ndotwi / pdf);
}
