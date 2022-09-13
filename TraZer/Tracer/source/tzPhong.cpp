
#include "../include/tzPhong.h"

//===================================================================================
tzPhong::tzPhong(void)
	:	tzIMaterial()
{
	mAmbientBRDF = std::make_shared<tzLambertian>();
	mDiffuseBRDF = std::make_shared<tzLambertian>();
	mSpecularBRDF = std::make_shared<tzGlossySpecular>();
}

//===================================================================================
tzPhong::tzPhong(const tzPhong& m)
	: 	tzIMaterial(m)
{
	if(m.mAmbientBRDF)
		mAmbientBRDF = std::dynamic_pointer_cast<tzLambertian>(m.mAmbientBRDF->clone());
	else  mAmbientBRDF = nullptr;
	
	if(m.mDiffuseBRDF)
		mDiffuseBRDF = std::dynamic_pointer_cast<tzLambertian>(m.mDiffuseBRDF->clone());
	else  mDiffuseBRDF = nullptr;

	//
	if (m.mSpecularBRDF)
	{
		mSpecularBRDF = std::dynamic_pointer_cast<tzGlossySpecular>(m.mSpecularBRDF->clone());
	}
	else
	{
		mSpecularBRDF = nullptr;
	}
}

//===================================================================================
std::shared_ptr<tzIMaterial> tzPhong::clone(void) const
{
	return (std::make_shared< tzPhong >(*this));
}	

//===================================================================================
tzPhong& tzPhong::operator= (const tzPhong& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzIMaterial::operator=(rhs);
	
	if (rhs.mAmbientBRDF)
		mAmbientBRDF = std::dynamic_pointer_cast<tzLambertian>(rhs.mAmbientBRDF->clone());
		
	if (rhs.mDiffuseBRDF)
		mDiffuseBRDF = std::dynamic_pointer_cast<tzLambertian>(rhs.mDiffuseBRDF->clone());

	if ( rhs.mSpecularBRDF )
	{
		mSpecularBRDF = std::dynamic_pointer_cast<tzGlossySpecular>(rhs.mSpecularBRDF->clone());
	}

	return (*this);
}

//===================================================================================
tzPhong::~tzPhong(void)
{
	
}

//===================================================================================
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
				L += ( mDiffuseBRDF->f(sr, wo, wi) + mSpecularBRDF->f( sr, wo, wi ) ) * sr.mWorld.mLights[j]->L(sr) * sr.mWorld.mLights[j]->G(sr) * ndotwi / sr.mWorld.mLights[j]->pdf(sr);
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
				L += (mDiffuseBRDF->f(sr, wo, wi) + mSpecularBRDF->f(sr, wo, wi)) * sr.mWorld.mLights[j]->L(sr) * ndotwi;
			}
		}
	}

	return (L);
}


