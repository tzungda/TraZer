#ifndef TZ_TRACE_SHADE_REC
#define TZ_TRACE_SHADE_REC

#include "Point3D.h"
#include "Vector3D.h"
#include "Normal.h"
#include "RGBColor.h"
#include "../include/tzRay.h"

class tzWorld;
class tzIMaterial;


class tzShadeRec
{
public:
	bool				mHitAnObject;
	Point3D				mLocalHitPoint;
	Normal				mNormal;
	RGBColor			mColor;
	tzWorld&			mWorld;
	float				mT;

	//
	tzIMaterial			*mMaterialPtr;
	Point3D				mHitPoint;
	tzRay				mRay;
	int					mDepth;
	Vector3D			mDir;


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

