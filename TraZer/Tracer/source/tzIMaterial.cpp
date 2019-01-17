
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

tzColor
tzIMaterial::shade(tzShadeRec& sr) {
	return (black);
}

//===================================================================================
tzColor tzIMaterial::areaLightShade( tzShadeRec& sr) const
{
	return black;
}

//===================================================================================
tzColor tzIMaterial::getLe(tzShadeRec &sr)
{
	return black;
}

//===================================================================================
tzColor tzIMaterial::pathShade(tzShadeRec& sr)
{
	return black;
}
