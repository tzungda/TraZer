
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
tzRGBColor tzIMaterial::area_light_shade(tzShadeRec& sr)
{
	return black;
}

//===================================================================================
tzRGBColor tzIMaterial::get_Le(tzShadeRec &sr)
{
	return black;
}

//===================================================================================
tzRGBColor tzIMaterial::path_shade(tzShadeRec& sr)
{
	return black;
}
