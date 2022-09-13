
#include "../include/tzWorld.h"
#include "../include/tzPointLight.h"


//===================================================================================
tzPointLight::tzPointLight(void)
	: 	tzILight(),
		mLs(1.0),
		mColor(1.0)
{}


//===================================================================================
tzPointLight::tzPointLight(const tzPointLight& a)
	: 	tzILight(a),
		mLs(a.mLs),
		mColor(a.mColor)
{}


//===================================================================================
std::shared_ptr<tzILight> tzPointLight::clone(void) const 
{
	return (std::make_shared< tzPointLight >(*this));
}	


//===================================================================================
tzPointLight& tzPointLight::operator= (const tzPointLight& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	mLs 	= rhs.mLs;
	mColor = rhs.mColor;
	
	return (*this);
}


//===================================================================================
tzPointLight::~tzPointLight(void) 
{}


//===================================================================================
tzVector3D tzPointLight::getDirection(tzShadeRec& s)
{
	return ((mLocation - s.mHitPoint).hat() );
}


//===================================================================================
bool tzPointLight::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();
	float d = (float)(mLocation - ray.mOrigin).length();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadowHit(ray, sr, t) && t < d)
		{
			return true;
		}
	}

	return false; 
}

//===================================================================================
tzColor tzPointLight::L(tzShadeRec& sr) 
{
	return (mLs * mColor);
}

//===================================================================================
void tzPointLight::set_location(const tzVector3D &loc)
{
	mLocation = loc;
}

