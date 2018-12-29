

#include "../include/tzAmbient.h"

// ---------------------------------------------------------------------- default constructor

tzAmbient::tzAmbient(void)
	: 	tzILight(),
		ls(1.0),
		color(1.0)			
{}


// ---------------------------------------------------------------------- copy constructor

tzAmbient::tzAmbient(const tzAmbient& a)
	: 	tzILight(a),
		ls(a.ls),
		color(a.color) 		
{}


// ---------------------------------------------------------------------- clone

tzILight* 
tzAmbient::clone(void) const {
	return (new tzAmbient(*this));
}	


// ---------------------------------------------------------------------- assignment operator

tzAmbient&
tzAmbient::operator= (const tzAmbient& rhs) {
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls 		= rhs.ls;
	color 	= rhs.color;
	
	return (*this);
}


// ---------------------------------------------------------------------- destructor																			

tzAmbient::~tzAmbient(void) {}


// ---------------------------------------------------------------------- get_direction	

Vector3D								
tzAmbient::get_direction(tzShadeRec& s) {
	return (Vector3D(0.0));
}


// ---------------------------------------------------------------------- L

RGBColor
tzAmbient::L(tzShadeRec& sr) {
	return (ls * color);
}




