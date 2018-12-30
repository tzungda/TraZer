
#include <math.h>    // for pow

#include "tzRGBColor.h"

// -------------------------------------------------------- default constructor

tzRGBColor::tzRGBColor(void)
	: r(0.0), g(0.0), b(0.0) 							
{}


// -------------------------------------------------------- constructor

tzRGBColor::tzRGBColor(float c)
	: r(c), g(c), b(c) 							
{}
								

// -------------------------------------------------------- constructor

tzRGBColor::tzRGBColor(float _r, float _g, float _b)
	: r(_r), g(_g), b(_b)
{}


// -------------------------------------------------------- copy constructor

tzRGBColor::tzRGBColor(const tzRGBColor& c)
	: r(c.r), g(c.g), b(c.b)
{} 				 
		

// -------------------------------------------------------- destructor

tzRGBColor::~tzRGBColor(void)
{}


// --------------------------------------------------------assignment operator

tzRGBColor&
tzRGBColor::operator= (const tzRGBColor& rhs) {
	if (this == &rhs)
		return (*this);

	r = rhs.r; g = rhs.g; b = rhs.b;

	return (*this);
}
 

// -------------------------------------------------------- powc
// raise each component to the specified power
// used for color filtering in Chapter 28

tzRGBColor
tzRGBColor::powc(float p) const {
	return (tzRGBColor((float)pow(r, p), (float)pow(g, p), (float)pow(b, p)));
}


