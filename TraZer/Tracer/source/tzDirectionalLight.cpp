

#include "../include/tzDirectionalLight.h"

//===================================================================================
tzDirectionalLight::tzDirectionalLight(void)
	:   tzILight(),
		mLs(1.0),
		mColor(1.0),
		mDir(0, 1, 0)			
{}


//===================================================================================
tzDirectionalLight::tzDirectionalLight(const tzDirectionalLight& dl)
	: 	tzILight(dl),
		mLs(dl.mLs),
		mColor(dl.mColor),
		mDir(dl.mDir)  		
{}


//===================================================================================
std::shared_ptr<tzILight> tzDirectionalLight::clone(void) const 
{
	return (std::make_shared< tzDirectionalLight >(*this));
}


//===================================================================================
tzDirectionalLight& tzDirectionalLight::operator= (const tzDirectionalLight& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	mLs		= rhs.mLs;
	mColor 	= rhs.mColor;
	mDir 	= rhs.mDir;

	return (*this);
}


//===================================================================================
tzDirectionalLight::~tzDirectionalLight(void) 
{
}


//===================================================================================
tzVector3D tzDirectionalLight::getDirection(tzShadeRec& sr)
{
	return (mDir);
}	

//===================================================================================
tzColor tzDirectionalLight::L(tzShadeRec& s) 
{
	return (mLs * mColor);
}

//===================================================================================
bool tzDirectionalLight::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadowHit(ray, sr, t))
		{
			return true;
		}
	}

	return false;
}


