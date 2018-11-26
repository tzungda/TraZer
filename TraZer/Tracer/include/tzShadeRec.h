#ifndef __TZ_SHADE_REC__
#define __TZ_SHADE_REC__

#include "Point3D.h"
#include "Vector3D.h"
#include "Normal.h"
#include "RGBColor.h"
#include "tzWrold.h"

class tzShadeRec
{
public:
	bool		mHitAnObject;
	Point3D		mLocalHitPoint;
	Normal		mNormal;
	RGBColor	mColor;
	tzWorld		mWorld;

	// constructor
	tzShadeRec( tzWorld &w );

	// copy consturctor
	tzShadeRec( const tzShadeRec &sr );

	// destructor
	~tzShadeRec( );

	// assignment operator
	tzShadeRec& operator= (const tzShadeRec &sr );

};

#endif

