#include "../include/tzWorld.h"
#include "../include/tzIMaterial.h"
#include "../include/tzShadeRec.h"


// ================================================================================
tzShadeRec::~tzShadeRec( )
{
}

// ================================================================================
tzShadeRec::tzShadeRec( tzWorld& w )
: mHitAnObject( false ), mLocalHitPoint(), mNormal(), mColor( 0.0f, 0.0f, 0.0f ), mWorld( w ), mT(0.0f), mHitPoint(), mMaterialPtr(NULL), mDepth(0), mDir(), mRay(), mU(0.0f), mV( 0.0f )
{
	mThreadId = 0;
	mMaxThreads = 1;
}

// ================================================================================
tzShadeRec::tzShadeRec(const tzShadeRec &sr)
:mHitAnObject( sr.mHitAnObject ), mLocalHitPoint( sr.mLocalHitPoint ), mNormal(sr.mNormal), mT(0.0f), mColor( sr.mColor ), mWorld( sr.mWorld), mHitPoint(sr.mHitPoint), mMaterialPtr(sr.mMaterialPtr), mDepth(sr.mDepth), mDir(sr.mDir), mRay(sr.mRay), mU(sr.mU), mV(sr.mV)
{
	mThreadId = sr.mThreadId;
	mMaxThreads = sr.mMaxThreads;
}

// ================================================================================
tzShadeRec&
tzShadeRec::operator= (const tzShadeRec& sr) {

	if (this == &sr)
		return (*this);

	mHitAnObject = sr.mHitAnObject;
	mLocalHitPoint = sr.mLocalHitPoint;
	mNormal = sr.mNormal;
	mColor = sr.mColor;
	mWorld = sr.mWorld;
	mT = sr.mT;
	mU = sr.mU;
	mV = sr.mV;
	mHitPoint = sr.mHitPoint;
	mRay = sr.mRay;
	mMaterialPtr = sr.mMaterialPtr;
	mDepth = sr.mDepth;
	mDir = sr.mDir;

	mThreadId = sr.mThreadId;
	mMaxThreads = sr.mMaxThreads;

	return (*this);
}