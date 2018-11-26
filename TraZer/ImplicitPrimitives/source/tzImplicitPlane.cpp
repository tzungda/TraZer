#include "ImplicitPrimitives\include\tzImplicitPlane.h"
#include <math.h>

const double tzPlane::mkEpsilon = 0.001;

// ================================================================================
tzPlane::tzPlane(void)
{
}

// ================================================================================
tzPlane::tzPlane(const Point3D &p, const Normal &n)
:mPoint(p), mNormal(n)
{
}

// ================================================================================
tzPlane::~tzPlane()
{
}


// geometric object interface
// ================================================================================
bool tzPlane::hit(const tzRay& ray, double &tmin, tzShadeRec &sr) const
{
	double t = ( mPoint - ray.o )*mNormal / (ray.d*mNormal);

	if ( t > mkEpsilon )
	{
		tmin = t;
		sr.mNormal = mNormal;
		sr.mLocalHitPoint = ray.o + t*ray.d;
	}
	else
	{
		return false;
	}
}