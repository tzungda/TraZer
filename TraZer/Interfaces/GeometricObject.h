#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

#include "tzRay.h"
#include "tzShadeRec.h"

class GeometricObject
{
public:
	virtual bool hit( const tzRay& ray, double &tmin, tzShadeRec &sr ) const = 0;

protected:
	RGBColor mColor;
};

#endif

