#ifndef TZ_GEOMETRIC_OBJECT
#define TZ_GEOMETRIC_OBJECT

#include "tzRay.h"
#include "tzShadeRec.h"

class tzIGeometricObject
{
public:
	virtual bool hit( const tzRay& ray, double &tmin, tzShadeRec &sr ) const = 0;

protected:
	RGBColor mColor;
};

#endif

