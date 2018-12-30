
#include "../include/tzWorld.h"
#include "../include/tzPointLight.h"


// ---------------------------------------------------------------------- default constructor

tzPointLight::tzPointLight(void)
	: 	tzILight(),
		ls(1.0),
		color(1.0)			
{}


// ---------------------------------------------------------------------- copy constructor

tzPointLight::tzPointLight(const tzPointLight& a)
	: 	tzILight(a),
		ls(a.ls),
		color(a.color) 		
{}


// ---------------------------------------------------------------------- clone

tzILight* 
tzPointLight::clone(void) const {
	return (new tzPointLight(*this));
}	


// ---------------------------------------------------------------------- assignment operator

tzPointLight&
tzPointLight::operator= (const tzPointLight& rhs) {
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls 		= rhs.ls;
	color 	= rhs.color;
	
	return (*this);
}


// ---------------------------------------------------------------------- destructor																			

tzPointLight::~tzPointLight(void) {}


// ---------------------------------------------------------------------- get_direction	

tzVector3D
tzPointLight::get_direction(tzShadeRec& s) {
	return ((location - s.mHitPoint).hat() );
}


//===================================================================================
bool tzPointLight::in_shadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t = 0.0f;
	int numObjects = (int)sr.mWorld.mObjects.size();
	float d = (float)(location - ray.o).length();

	for (int j = 0; j < numObjects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadow_hit(ray, t) && t < d)
		{
			return true;
		}
	}

	return false; 
}

// ---------------------------------------------------------------------- L

tzRGBColor
tzPointLight::L(tzShadeRec& sr) {
	return (ls * color);
}

void tzPointLight::set_location(const tzVector3D &loc)
{
	location = loc;
}

