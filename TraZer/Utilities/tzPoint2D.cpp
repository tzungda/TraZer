
#include "tzPoint2D.h"

// ------------------------------------------------ default constructor

tzPoint2D::tzPoint2D(void)
	: x(0.0), y(0.0)
{}


// ------------------------------------------------ constructor

tzPoint2D::tzPoint2D(const float arg)
	: x(arg), y(arg)
{}


// ------------------------------------------------ constructor

tzPoint2D::tzPoint2D(const float x1, const float y1)
	: x(x1), y(y1)
{}


// ------------------------------------------------ copy constructor

tzPoint2D::tzPoint2D(const tzPoint2D& p)
	: x(p.x), y(p.y)
{}


// ------------------------------------------------ destructor

tzPoint2D::~tzPoint2D(void) {}


// ------------------------------------------------ assignment operator

tzPoint2D& 							
tzPoint2D::operator= (const tzPoint2D& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x;
	y = rhs.y;

	return (*this);
}
