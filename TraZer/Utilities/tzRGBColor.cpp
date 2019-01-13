
#include <math.h>    // for pow

#include "tzRGBColor.h"

//===================================================================================
tzRGBColor::tzRGBColor(void)
	: r(0.0f), g(0.0f), b(0.0f), a(1.0f) 							
{}


//===================================================================================
tzRGBColor::tzRGBColor(float c)
	: r(c), g(c), b(c), a(1.0f) 							
{}
								

//===================================================================================
tzRGBColor::tzRGBColor(float _r, float _g, float _b)
	: r(_r), g(_g), b(_b), a(1.0f)
{}

//===================================================================================
tzRGBColor::tzRGBColor(float _r, float _g, float _b, float _a)
:r(_r), g(_g), b(_b), a(_a)
{
}

//===================================================================================
tzRGBColor::tzRGBColor(const tzRGBColor& c)
	: r(c.r), g(c.g), b(c.b), a(c.a)
{} 				 
		

//===================================================================================
tzRGBColor::~tzRGBColor(void)
{}


//===================================================================================
tzRGBColor& tzRGBColor::operator= (const tzRGBColor& rhs) 
{
	if (this == &rhs)
		return (*this);

	r = rhs.r; g = rhs.g; b = rhs.b, a = rhs.a;

	return (*this);
}
 

//===================================================================================
tzRGBColor tzRGBColor::powc(float p) const 
{
	return (tzRGBColor((float)pow(r, p), (float)pow(g, p), (float)pow(b, p)));
}


