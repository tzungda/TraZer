
#include "../include/tzMatteSV.h"

//===================================================================================
tzMatteSV::tzMatteSV(void)
	:	tzIMaterial()
{
	mAmbientBRDF = std::make_shared< tzLambertianSV >();
	mDiffuseBRDF = std::make_shared< tzLambertianSV >();
}

//===================================================================================
tzMatteSV::tzMatteSV(const tzMatteSV& m)
	: 	tzIMaterial(m)
{
	if(m.mAmbientBRDF)
		mAmbientBRDF = std::dynamic_pointer_cast<tzLambertianSV>( m.mAmbientBRDF->clone() );
	else  mAmbientBRDF = nullptr;
	
	if(m.mDiffuseBRDF)
		mDiffuseBRDF = std::dynamic_pointer_cast<tzLambertianSV>( m.mDiffuseBRDF->clone() );
	else  mDiffuseBRDF = nullptr;
}

//===================================================================================
std::shared_ptr<tzIMaterial> tzMatteSV::clone(void) const
{
	return (std::make_shared< tzMatteSV >(*this));
}	

//===================================================================================
tzMatteSV& tzMatteSV::operator= (const tzMatteSV& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzIMaterial::operator=(rhs);

	if (rhs.mAmbientBRDF)
		mAmbientBRDF = std::dynamic_pointer_cast<tzLambertianSV>( rhs.mAmbientBRDF->clone() );
		
	if (rhs.mDiffuseBRDF)
		mDiffuseBRDF = std::dynamic_pointer_cast<tzLambertianSV>( rhs.mDiffuseBRDF->clone() );

	return (*this);
}

//===================================================================================
tzMatteSV::~tzMatteSV(void)
{
}

//===================================================================================
tzColor tzMatteSV::shade(tzShadeRec& sr) 
{
	tzVector3D 	wo 			= -sr.mRay.mDirection;
	tzColor 	L 			= mAmbientBRDF->rho(sr, wo) * sr.mWorld.mAmbientPtr->L(sr);
	int 		num_lights	= (int)sr.mWorld.mLights.size();
	
	for (int j = 0; j < num_lights; j++) 
	{
		std::shared_ptr<tzILight> light_ptr = sr.mWorld.mLights[j];
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
