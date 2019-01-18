#ifndef TZ_TRACE_SHADE_REC
#define TZ_TRACE_SHADE_REC

#include "tzPoint3D.h"
#include "tzVector3D.h"
#include "tzNormal.h"
#include "tzColor.h"
#include "../include/tzRay.h"

class tzWorld;
class tzIMaterial;


class tzShadeRec
{
public:
	bool				mHitAnObject;
	tzPoint3D			mLocalHitPoint;
	tzNormal			mNormal;
	tzColor				mColor;
	tzWorld&			mWorld;
	float				mT;
	float				mU;
	float				mV;

	//
	tzIMaterial			*mMaterialPtr;
	tzPoint3D			mHitPoint;
	tzRay				mRay;
	int					mDepth;
	tzVector3D			mDir;

	//
	unsigned int		mThreadId;
	unsigned int		mMaxThreads;


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

