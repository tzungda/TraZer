

#include "../include/tzDirectional.h"

//===================================================================================
tzDirectional::tzDirectional(void)
	:   tzILight(),
		mLs(1.0),
		mColor(1.0),
		mDir(0, 1, 0)			
{}


//===================================================================================
tzDirectional::tzDirectional(const tzDirectional& dl)
	: 	tzILight(dl),
		mLs(dl.mLs),
		mColor(dl.mColor),
		mDir(dl.mDir)  		
{}


//===================================================================================
tzILight* tzDirectional::clone(void) const 
{
	return (new tzDirectional(*this));
}


//===================================================================================
tzDirectional& tzDirectional::operator= (const tzDirectional& rhs)
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
tzDirectional::~tzDirectional(void) 
{
}


//===================================================================================
tzVector3D tzDirectional::getDirection(tzShadeRec& sr)
{
	return (mDir);
}	

//===================================================================================
tzColor tzDirectional::L(tzShadeRec& s) 
{
	return (mLs * mColor);
}

//===================================================================================
bool tzDirectional::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadowHit(ray, t))
		{
			return true;
		}
	}

	return false;
}


