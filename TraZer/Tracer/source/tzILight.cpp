
#include "../include/tzILight.h"
#include "../include/tzConstants.h"

//===================================================================================
tzILight::tzILight(void) 
:mCastsShadows(false)
{}

//===================================================================================
tzILight::tzILight(const tzILight& ls) 
:mCastsShadows(ls.mCastsShadows)
{}

//===================================================================================
tzILight& tzILight::operator= (const tzILight& rhs) 
{
	if (this == &rhs)
		return (*this);

	return (*this);
}

//===================================================================================
tzILight::~tzILight(void) 
{
}

//===================================================================================
tzColor tzILight::L( tzShadeRec& s) 
{
	return (black);
}

//===================================================================================
float tzILight::G(const tzShadeRec& sr) const
{
	return 1.0f;
}

//===================================================================================
bool tzILight::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	return false;
}

//===================================================================================
void tzILight::setCastsShadows(bool castsShadow)
{
	mCastsShadows = castsShadow;
}

//===================================================================================
bool tzILight::castsShadow() const
{
	return mCastsShadows;
}

//===================================================================================
float tzILight::pdf(const tzShadeRec &sr) const
{
	return 1.0f;
}
