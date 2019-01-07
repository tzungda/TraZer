

#include "../include/tzAmbient.h"

//===================================================================================
tzAmbient::tzAmbient(void)
	: 	tzILight(),
		ls(1.0),
		color(1.0)			
{}


//===================================================================================
tzAmbient::tzAmbient(const tzAmbient& a)
	: 	tzILight(a),
		ls(a.ls),
		color(a.color) 		
{}


//===================================================================================
tzILight* tzAmbient::clone(void) const
{
	return (new tzAmbient(*this));
}	


//===================================================================================
tzAmbient& tzAmbient::operator= (const tzAmbient& rhs) 
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls 		= rhs.ls;
	color 	= rhs.color;
	
	return (*this);
}


//===================================================================================
tzAmbient::~tzAmbient(void) {}


//===================================================================================
tzVector3D tzAmbient::getDirection(  tzShadeRec& s)
{
	return (tzVector3D(0.0));
}


//===================================================================================
tzRGBColor tzAmbient::L(  tzShadeRec& sr) 
{
	return (ls * color);
}





