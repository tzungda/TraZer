
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

RGBColor
tzMatte::shade(tzShadeRec& sr) {
	Vector3D 	wo 			= -sr.mRay.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights	= (int)sr.mWorld.mLights.size();
	
	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.mWorld.mLights[j]->get_direction(sr);    
		float ndotwi = (float)(sr.mNormal * wi);
	
		if (ndotwi > 0.0) 
		{
			// check if it's in shadow
			bool in_shadow = false;
			if ( sr.mWorld.mLights[j]->castsShadow() )
			{
				tzRay shadowRay( sr.mHitPoint, wi );
				in_shadow = sr.mWorld.mLights[j]->in_shadow( shadowRay, sr );
			}

			if ( !in_shadow )
			{
				L += diffuse_brdf->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * ndotwi;
			}
		}
	}
	
	return (L);
}

//===================================================================================
RGBColor tzMatte::area_light_shade(tzShadeRec &sr)
{
	Vector3D 	wo = -sr.mRay.d;
	RGBColor 	L = ambient_brdf->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights = (int)sr.mWorld.mLights.size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.mWorld.mLights[j]->get_direction(sr);
		float ndotwi = (float)(sr.mNormal * wi);

		if (ndotwi > 0.0)
		{
			// check if it's in shadow
			bool in_shadow = false;
			if (sr.mWorld.mLights[j]->castsShadow())
			{
				tzRay shadowRay(sr.mHitPoint, wi);
				in_shadow = sr.mWorld.mLights[j]->in_shadow(shadowRay, sr);
			}

			if (!in_shadow)
			{
				L += diffuse_brdf->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
			}
		}
	}

	return (L);
}


