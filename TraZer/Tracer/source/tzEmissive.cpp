
#include "../include/tzEmissive.h"
 
// ---------------------------------------------------------------- default constructor

tzEmissive::tzEmissive(void)
	:	tzIMaterial(), ls( 1.0f ), ce( white )
{}



// ---------------------------------------------------------------- copy constructor

tzEmissive::tzEmissive(const tzEmissive& m)
	: 	tzIMaterial(m), ls(m.emissiveRadiance()), ce(m.emissiveColor())
{
}


// ---------------------------------------------------------------- clone

tzIMaterial*										
tzEmissive::clone(void) const {
	return (new tzEmissive(*this));
}	


// ---------------------------------------------------------------- assignment operator

tzEmissive&
tzEmissive::operator= (const tzEmissive& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzIMaterial::operator=(rhs);
	
	ls = rhs.emissiveRadiance();
	ce = rhs.emissiveColor();

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzEmissive::~tzEmissive(void) 
{	
}


//===================================================================================
tzColor tzEmissive::shade(tzShadeRec& sr)
{
	return black;	
}

//===================================================================================
tzColor tzEmissive::areaLightShade( tzShadeRec& sr) const
{
	if ( -sr.mNormal * sr.mRay.d > 0.0f )
	{
		return ls*ce;
	}
	
	return black;
}

//===================================================================================
tzColor tzEmissive::getLe( tzShadeRec &sr )
{
	return ls*ce;
}


//===================================================================================
float tzEmissive::emissiveRadiance() const
{
	return ls;
}

//===================================================================================
tzColor tzEmissive::emissiveColor() const
{
	return ce;
}

//===================================================================================
void tzEmissive::scaleRadiance(const float k)
{
	ls = k;
}

//===================================================================================
void tzEmissive::set_ce(const tzColor &c)
{
	ce = c;
}

//===================================================================================
void tzEmissive::set_ce(const float r, const float g, const float b)
{
	ce = tzColor(r, g, b);
}

//===================================================================================
void tzEmissive::set_ce(const float c)
{
	ce = tzColor(c);
}
