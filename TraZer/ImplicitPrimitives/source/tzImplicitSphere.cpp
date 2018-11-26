#include "ImplicitPrimitives\include\tzImplicitSphere.h"
#include <math.h>

#include "math.h"

const double tzSphere::kEpsilon = 0.001;

// ---------------------------------------------------------------- default constructor

tzSphere::tzSphere(void)
	: GeometricObject(),
	center(0.0),
	radius(1.0)
{}


// ---------------------------------------------------------------- constructor

tzSphere::tzSphere(Point3D c, double r)
	:center(c),radius(r)
{}


// ---------------------------------------------------------------- clone

tzSphere*
tzSphere::clone(void) const {
	return (new tzSphere(*this));
}


// ---------------------------------------------------------------- copy constructor

tzSphere::tzSphere(const tzSphere& sphere)
:center( sphere.center), radius( sphere.radius)
{}



// ---------------------------------------------------------------- assignment operator

tzSphere&
tzSphere::operator= (const tzSphere& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center = rhs.center;
	radius = rhs.radius;

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzSphere::~tzSphere(void) {}


//---------------------------------------------------------------- hit

bool
tzSphere::hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const {
	double 		t;
	Vector3D	temp = ray.o - center;
	double 		a = ray.d * ray.d;
	double 		b = 2.0 * temp * ray.d;
	double 		c = temp * temp - radius * radius;
	double 		disc = b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal = (temp + t * ray.d) / radius;
			sr.mLocalHitPoint = ray.o + t * ray.d;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			sr.mNormal = (temp + t * ray.d) / radius;
			sr.mLocalHitPoint = ray.o + t * ray.d;
			return (true);
		}
	}

	return (false);
}
