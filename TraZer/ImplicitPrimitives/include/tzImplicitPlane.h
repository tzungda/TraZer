#ifndef __TZ_IMPLICITPLANE__
#define __TZ_IMPLICITPLANE__

#include "Interfaces\GeometricObject.h"

class tzPlane: public GeometricObject
{
public:
	tzPlane( void );
	tzPlane( const Point3D &p, const Normal &n );
	virtual ~tzPlane();

	// geometric object interface
	virtual bool hit(const tzRay& ray, double &tmin, tzShadeRec &sr) const;

private:
	Point3D		mPoint;
	Normal		mNormal;
	static const double mkEpsilon;
};

#endif
