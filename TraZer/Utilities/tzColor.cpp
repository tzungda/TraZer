
#include <math.h>    // for pow

#include "tzColor.h"

//===================================================================================
tzColor::tzColor(void)
	: r(0.0f), g(0.0f), b(0.0f), a(1.0f) 							
{}


//===================================================================================
tzColor::tzColor(float c)
	: r(c), g(c), b(c), a(1.0f) 							
{}
								

//===================================================================================
tzColor::tzColor(float _r, float _g, float _b)
	: r(_r), g(_g), b(_b), a(1.0f)
{}

//===================================================================================
tzColor::tzColor(float _r, float _g, float _b, float _a)
:r(_r), g(_g), b(_b), a(_a)
{
}

//===================================================================================
tzColor::tzColor(const tzColor& c)
	: r(c.r), g(c.g), b(c.b), a(c.a)
{} 				 
		

//===================================================================================
tzColor::~tzColor(void)
{}


//===================================================================================
tzColor& tzColor::operator= (const tzColor& rhs) 
{
	if (this == &rhs)
		return (*this);

	r = rhs.r; g = rhs.g; b = rhs.b, a = rhs.a;

	return (*this);
}
 

//===================================================================================
tzColor tzColor::powc(float p) const 
{
	return (tzColor((float)pow(r, p), (float)pow(g, p), (float)pow(b, p)));
}


