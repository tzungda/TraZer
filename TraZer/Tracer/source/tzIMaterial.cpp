
#include "../include/tzIMaterial.h"

// ---------------------------------------------------------------- default constructor

tzIMaterial::tzIMaterial(void) {}


// ---------------------------------------------------------------- copy constructor

tzIMaterial::tzIMaterial(const tzIMaterial& m) {}



// ---------------------------------------------------------------- assignment operator

tzIMaterial&
tzIMaterial::operator= (const tzIMaterial& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzIMaterial::~tzIMaterial(void)
{}


// ---------------------------------------------------------------- shade

tzRGBColor
tzIMaterial::shade(tzShadeRec& sr) {
	return (black);
}

//===================================================================================
tzRGBColor tzIMaterial::areaLightShade( tzShadeRec& sr) const
{
	return black;
}

//===================================================================================
tzRGBColor tzIMaterial::getLe(tzShadeRec &sr)
{
	return black;
}

//===================================================================================
tzRGBColor tzIMaterial::pathShade(tzShadeRec& sr)
{
	return black;
}
