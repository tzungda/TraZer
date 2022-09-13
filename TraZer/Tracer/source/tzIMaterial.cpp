
#include "../include/tzIMaterial.h"

//===================================================================================
tzIMaterial::tzIMaterial(void) 
{
}

//===================================================================================
tzIMaterial::tzIMaterial(const tzIMaterial& m) 
{
}

//===================================================================================
tzIMaterial& tzIMaterial::operator= (const tzIMaterial& rhs) 
{
	if (this == &rhs)
		return (*this);

	return (*this);
}

//===================================================================================
tzIMaterial::~tzIMaterial(void)
{}

//===================================================================================
tzColor tzIMaterial::shade(tzShadeRec& sr) 
{
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
