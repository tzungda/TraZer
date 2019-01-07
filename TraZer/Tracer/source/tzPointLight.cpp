
#include "../include/tzWorld.h"
#include "../include/tzPointLight.h"


//===================================================================================
tzPointLight::tzPointLight(void)
	: 	tzILight(),
		ls(1.0),
		color(1.0)			
{}


//===================================================================================
tzPointLight::tzPointLight(const tzPointLight& a)
	: 	tzILight(a),
		ls(a.ls),
		color(a.color) 		
{}


//===================================================================================
tzILight* tzPointLight::clone(void) const 
{
	return (new tzPointLight(*this));
}	


//===================================================================================
tzPointLight& tzPointLight::operator= (const tzPointLight& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls 		= rhs.ls;
	color 	= rhs.color;
	
	return (*this);
}


//===================================================================================
tzPointLight::~tzPointLight(void) 
{}


//===================================================================================
tzVector3D tzPointLight::getDirection(tzShadeRec& s)
{
	return ((location - s.mHitPoint).hat() );
}


//===================================================================================
bool tzPointLight::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();
	float d = (float)(location - ray.o).length();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadowHit(ray, t) && t < d)
		{
			return true;
		}
	}

	return false; 
}

//===================================================================================
tzRGBColor tzPointLight::L(tzShadeRec& sr) 
{
	return (ls * color);
}

//===================================================================================
void tzPointLight::set_location(const tzVector3D &loc)
{
	location = loc;
}

