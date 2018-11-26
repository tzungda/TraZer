#include "tzShadeRec.h"

// ================================================================================
tzShadeRec::~tzShadeRec( )
{
}

// ================================================================================
tzShadeRec::tzShadeRec( tzWorld& w )
: mHitAnObject( false ), mLocalHitPoint(), mNormal(), mColor( 0.0f, 0.0f, 0.0f ), mWorld( w )
{
}

// ================================================================================
tzShadeRec::tzShadeRec(const tzShadeRec &sr)
{
	mHitAnObject = sr.mHitAnObject;
	mLocalHitPoint = sr.mLocalHitPoint;
	mNormal = sr.mNormal;
	mColor = sr.mColor;
	mWorld = sr.mWorld;
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