
#include "../include/tzPlane.h"

const double tzPlane::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

tzPlane::tzPlane(void)
	: 	tzIGeometricObject(),
		a(0.0),
		n(0, 1, 0)						
{}


// ----------------------------------------------------------------------  constructor

tzPlane::tzPlane(const tzPoint3D& point, const tzNormal& normal)
	:	tzIGeometricObject(),
		a(point),
		n(normal)
{
		n.normalize();
}


// ---------------------------------------------------------------- copy constructor

tzPlane::tzPlane(const tzPlane& plane)
	:	tzIGeometricObject(plane),
		a(plane.a),
		n(plane.n) 				
{}


// ---------------------------------------------------------------- clone

tzPlane*
tzPlane::clone(void) const {
	return (new tzPlane(*this));
}


// ---------------------------------------------------------------- assignment operator

tzPlane&
tzPlane::operator= (const tzPlane& rhs)	{
	
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);

	a = rhs.a;
	n = rhs.n;

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzPlane::~tzPlane(void)
{}

//===================================================================================
bool tzPlane::shadowHit(const tzRay &ray, float &tmin) const
{
	float t = (float)((a - ray.o) * n / (ray.d * n));

	if ( t > kEpsilon )
	{
		tmin = t;
		return true;
	}

	return false;
}

// ----------------------------------------------------------------- hit

bool 															 
tzPlane::hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const {
	float t = (float)((a - ray.o) * n / (ray.d * n)); 
														
	if (t > kEpsilon) {
		tmin = t;
		sr.mNormal = n;
		sr.mLocalHitPoint = ray.o + t * ray.d;
		
		return (true);	
	}

	return(false);
}


