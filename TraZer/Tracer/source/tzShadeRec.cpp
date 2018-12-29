#include "../include/tzWorld.h"
#include "../include/tzIMaterial.h"
#include "../include/tzShadeRec.h"


// ================================================================================
tzShadeRec::~tzShadeRec( )
{
}

// ================================================================================
tzShadeRec::tzShadeRec( tzWorld& w )
: mHitAnObject( false ), mLocalHitPoint(), mNormal(), mColor( 0.0f, 0.0f, 0.0f ), mWorld( w ), mT(0.0f), mHitPoint(), mMaterialPtr(NULL), mDepth(0), mDir(), mRay()
{
}

// ================================================================================
tzShadeRec::tzShadeRec(const tzShadeRec &sr)
:mHitAnObject( sr.mHitAnObject ), mLocalHitPoint( sr.mLocalHitPoint ), mNormal(sr.mNormal), mT(0.0f), mColor( sr.mColor ), mWorld( sr.mWorld), mHitPoint(sr.mHitPoint), mMaterialPtr(sr.mMaterialPtr), mDepth(sr.mDepth), mDir(sr.mDir), mRay(sr.mRay)
{
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

	return (*this);
}