
#include "../include/tzILight.h"
#include "../include/tzConstants.h"

// ---------------------------------------------------------------------- default constructor

tzILight::tzILight(void) 
:mCastsShadows(false)
{}

// ---------------------------------------------------------------------- copy constructor

tzILight::tzILight(const tzILight& ls) 
:mCastsShadows(ls.mCastsShadows)
{}


// ---------------------------------------------------------------------- assignment operator

tzILight&
tzILight::operator= (const tzILight& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------------- destructor

tzILight::~tzILight(void) {}



// ---------------------------------------------------------------------- L
// returns the radiance

tzRGBColor
tzILight::L(tzShadeRec& s) {
	return (black);
}

//===================================================================================
float tzILight::G(const tzShadeRec& sr) const
{
	return 1.0f;
}

//===================================================================================
bool tzILight::in_shadow(const tzRay &ray, const tzShadeRec &sr) const
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
