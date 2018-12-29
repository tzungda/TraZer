
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

RGBColor
tzIMaterial::shade(tzShadeRec& sr) {
	return (black);
}

//===================================================================================
RGBColor tzIMaterial::area_light_shade(tzShadeRec& sr)
{
	return black;
}

//===================================================================================
RGBColor tzIMaterial::get_Le(tzShadeRec &sr)
{
	return black;
}
