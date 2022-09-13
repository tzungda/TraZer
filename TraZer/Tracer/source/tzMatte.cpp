
#include "../include/tzMatte.h"

//===================================================================================
tzMatte::tzMatte(void)
	:	tzIMaterial()
{
	mAmbientBRDF = std::make_shared< tzLambertian >();
	mDiffuseBRDF = std::make_shared< tzLambertian >();
}

//===================================================================================
tzMatte::tzMatte(const tzMatte& m)
	: 	tzIMaterial(m)
{
	if(m.mAmbientBRDF)
		mAmbientBRDF = std::dynamic_pointer_cast<tzLambertian>( m.mAmbientBRDF->clone() );
	else  mAmbientBRDF = nullptr;
	
	if(m.mDiffuseBRDF)
		mDiffuseBRDF = std::dynamic_pointer_cast<tzLambertian>(m.mDiffuseBRDF->clone());
	else  mDiffuseBRDF = nullptr;
}

//===================================================================================
std::shared_ptr<tzIMaterial> tzMatte::clone(void) const 
{
	return (std::make_shared<tzMatte>(*this));
}	

//===================================================================================
tzMatte& tzMatte::operator= (const tzMatte& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzIMaterial::operator=(rhs);


	if (rhs.mAmbientBRDF)
		mAmbientBRDF = std::dynamic_pointer_cast<tzLambertian>(rhs.mAmbientBRDF->clone());
		

	if (rhs.mDiffuseBRDF)
		mDiffuseBRDF = std::dynamic_pointer_cast<tzLambertian>(rhs.mDiffuseBRDF->clone());

	return (*this);
}

//===================================================================================
tzMatte::~tzMatte(void) 
{
}

//===================================================================================
tzColor tzMatte::shade(tzShadeRec& sr) 
{
	tzVector3D 	wo 			= -sr.mRay.mDirection;
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
	tzVector3D 	wo = -sr.mRay.mDirection;
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
	tzVector3D 	wo = -sr.mRay.mDirection;
	tzVector3D 	wi;
	float 		pdf;
	tzColor 	f = mDiffuseBRDF->sampleF(sr, wo, wi, pdf);
	float 		ndotwi = sr.mNormal * wi;
	tzRay 		reflected_ray(sr.mHitPoint, wi);

	return (f * sr.mWorld.mTracerPtr->traceRay(reflected_ray, sr.mDepth + 1) * ndotwi / pdf);
}
