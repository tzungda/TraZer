#include "../include/tzWorld.h"
#include "../include/tzIMaterial.h"
#include "../include/tzShadeRec.h"



// ================================================================================
tzShadeRec::~tzShadeRec( )
{
}

// ================================================================================
tzShadeRec::tzShadeRec( tzWorld& w )
: mHitAnObject( false ), mLocalHitPoint(), mNormal(), mColor( 0.0f, 0.0f, 0.0f ), mWorld( w ), mT(0.0f), mHitPoint(), mDepth(0), mDir(), mRay(), mU(0.0f), mV( 0.0f )
{
	mThreadId = 0;
	mMaxThreads = 1;
	for ( int i = 0; i < MAX_THREADS; i++ )
	{
		mMaterialPtr[i] = nullptr;
	}
}

// ================================================================================
tzShadeRec::tzShadeRec(const tzShadeRec &sr)
:mHitAnObject( sr.mHitAnObject ), mLocalHitPoint( sr.mLocalHitPoint ), mNormal(sr.mNormal), mT(0.0f), mColor( sr.mColor ), mWorld( sr.mWorld), mHitPoint(sr.mHitPoint), mDepth(sr.mDepth), mDir(sr.mDir), mRay(sr.mRay), mU(sr.mU), mV(sr.mV)
{
	mThreadId = sr.mThreadId;
	mMaxThreads = sr.mMaxThreads;
	mMaterialPtr[mThreadId] = sr.mMaterialPtr[mThreadId];
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
	for ( int i = 0; i < MAX_THREADS; i++ )
	{
		mMaterialPtr[i] = sr.mMaterialPtr[i];
	}
	mDepth = sr.mDepth;
	mDir = sr.mDir;

	mThreadId = sr.mThreadId;
	mMaxThreads = sr.mMaxThreads;

	return (*this);
}