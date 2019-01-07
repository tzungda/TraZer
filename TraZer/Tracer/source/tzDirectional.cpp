

#include "../include/tzDirectional.h"

//===================================================================================
tzDirectional::tzDirectional(void)
	:   tzILight(),
		ls(1.0),
		color(1.0),
		dir(0, 1, 0)			
{}


//===================================================================================
tzDirectional::tzDirectional(const tzDirectional& dl)
	: 	tzILight(dl),
		ls(dl.ls),
		color(dl.color),
		dir(dl.dir)  		
{}


//===================================================================================
tzILight* tzDirectional::clone(void) const 
{
	return (new tzDirectional(*this));
}


//===================================================================================
tzDirectional& tzDirectional::operator= (const tzDirectional& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls		= rhs.ls;
	color 	= rhs.color;
	dir 	= rhs.dir;

	return (*this);
}


//===================================================================================
tzDirectional::~tzDirectional(void) 
{
}


//===================================================================================
tzVector3D tzDirectional::getDirection(tzShadeRec& sr)
{
	return (dir);
}	

//===================================================================================
tzRGBColor tzDirectional::L(tzShadeRec& s) 
{
	return (ls * color);
}

//===================================================================================
bool tzDirectional::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadowHit(ray, t))
		{
			return true;
		}
	}

	return false;
}


