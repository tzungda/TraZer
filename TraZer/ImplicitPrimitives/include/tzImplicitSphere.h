#ifndef __TZ_IMPLICITSPHERE__
#define __TZ_IMPLICITSPHERE__

#include "Interfaces\GeometricObject.h"

class tzSphere : public GeometricObject {

public:

	tzSphere(void);   									// Default constructor

	tzSphere(Point3D center, double r);					// Constructor 

	tzSphere(const tzSphere& sphere); 						// Copy constructor

	virtual tzSphere* 									// Virtual copy constructor
		clone(void) const;

	virtual												// Destructor
		~tzSphere(void);

	tzSphere& 											// assignment operator
		operator= (const tzSphere& sphere);

	void
		set_center(const Point3D& c);

	void
		set_center(const double x, const double y, const double z);

	void
		set_radius(const double r);

	virtual bool
		hit(const tzRay& ray, double& t, tzShadeRec& s) const;

private:

	Point3D 	center;   			// center coordinates as a point  
	double 		radius;				// the radius 

	static const double kEpsilon;   // for shadows and secondary rays
};

inline void
tzSphere::set_center(const Point3D& c) {
	center = c;
}

inline void
tzSphere::set_center(const double x, const double y, const double z) {
	center.x = x;
	center.y = y;
	center.z = z;
}

inline void
tzSphere::set_radius(const double r) {
	radius = r;
}

#endif
