#include "tzRay.h"

// ================================================================================
tzRay::~tzRay(void) 
{
}

// ================================================================================
tzRay::tzRay(void)
	: o(0.0),
	d(0.0, 0.0, 1.0),
	depth(0)
{}

// ================================================================================
tzRay::tzRay(const tzPoint3D& origin, const tzVector3D& dir)
	: o(origin), d(dir), depth(0)
{}

// ================================================================================
tzRay::tzRay(const tzRay& ray)
	: o(ray.o),
	d(ray.d),
	depth(ray.depth)
{}

// ================================================================================
tzRay&
tzRay::operator= (const tzRay& rhs) {

	if (this == &rhs)
		return (*this);

	o = rhs.o;
	d = rhs.d;
	depth = rhs.depth;

	return (*this);
}

