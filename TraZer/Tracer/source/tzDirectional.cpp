

#include "../include/tzDirectional.h"

// ---------------------------------------------------------------------- default constructor

tzDirectional::tzDirectional(void)
	:   tzILight(),
		ls(1.0),
		color(1.0),
		dir(0, 1, 0)			
{}


// ---------------------------------------------------------------------- dopy constructor

tzDirectional::tzDirectional(const tzDirectional& dl)
	: 	tzILight(dl),
		ls(dl.ls),
		color(dl.color),
		dir(dl.dir)  		
{}


// ---------------------------------------------------------------------- clone

tzILight* 
tzDirectional::clone(void) const {
	return (new tzDirectional(*this));
}


// ---------------------------------------------------------------------- assignment operator

tzDirectional&
tzDirectional::operator= (const tzDirectional& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls		= rhs.ls;
	color 	= rhs.color;
	dir 	= rhs.dir;

	return (*this);
}


// ---------------------------------------------------------------------- destructor																			

tzDirectional::~tzDirectional(void) {}


// ---------------------------------------------------------------------- get_direction
// as this function is virtual, it shouldn't be inlined 

tzVector3D
tzDirectional::get_direction(tzShadeRec& sr) {
	return (dir);
}	

// ------------------------------------------------------------------------------  L

tzRGBColor
tzDirectional::L(tzShadeRec& s) {
	return (ls * color);
}

//===================================================================================
bool tzDirectional::in_shadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadow_hit(ray, t))
		{
			return true;
		}
	}

	return false;
}


