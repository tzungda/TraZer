
#include "../include/tzPhong.h"

// ---------------------------------------------------------------- default constructor

tzPhong::tzPhong(void)
	:	tzIMaterial(),
		ambient_brdf(new tzLambertian),
		diffuse_brdf(new tzLambertian),
		specular_brdf( new tzGlossySpecular )
{}



// ---------------------------------------------------------------- copy constructor

tzPhong::tzPhong(const tzPhong& m)
	: 	tzIMaterial(m)
{
	if(m.ambient_brdf)
		ambient_brdf = m.ambient_brdf->clone(); 
	else  ambient_brdf = NULL;
	
	if(m.diffuse_brdf)
		diffuse_brdf = m.diffuse_brdf->clone(); 
	else  diffuse_brdf = NULL;

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

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}
	
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}

	if (specular_brdf )
	{
		delete specular_brdf;
		specular_brdf = NULL;
	}
}


// ---------------------------------------------------------------- shade

tzRGBColor
tzPhong::shade(tzShadeRec& sr) {
	tzVector3D 	wo 			= -sr.mRay.d;
	tzRGBColor	ambientColor = ambient_brdf->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	tzRGBColor 	L 			= ambientColor;
	int 		num_lights	= (int)sr.mWorld.mLights.size();
	
	for (int j = 0; j < num_lights; j++) {
		tzVector3D wi = sr.mWorld.mLights[j]->get_direction(sr);
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
				// L += diffuse_brdf->f(sr, wo, wi) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
				L += ( diffuse_brdf->f(sr, wo, wi) + specular_brdf->f( sr, wo, wi ) ) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
			}
		}
	}
	
	return (L);
}

//===================================================================================
tzRGBColor tzPhong::area_light_shade(tzShadeRec &sr)
{
	tzVector3D 	wo = -sr.mRay.d;
	tzRGBColor	ambientColor = ambient_brdf->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	tzRGBColor 	L = ambientColor;
	int 		num_lights = (int)sr.mWorld.mLights.size();

	for (int j = 0; j < num_lights; j++) 
	{
		tzVector3D wi = sr.mWorld.mLights[j]->get_direction(sr);
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
				L += (diffuse_brdf->f(sr, wo, wi) + specular_brdf->f(sr, wo, wi)) * sr.mWorld.mLights[j]->L(sr) * ndotwi;
			}
		}
	}

	return (L);
}


