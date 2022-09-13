
#include "../include/tzWorld.h"
#include "../include/tzAmbientOccluder.h"

//===================================================================================
tzAmbientOccluder::tzAmbientOccluder(void)
	: 	tzILight(),
		mLs(1.0),
		mColor(1.0),
		mMinAmount( 0.0f ),
		mSamplerPtr( nullptr )
{}


//===================================================================================
tzAmbientOccluder::tzAmbientOccluder(const tzAmbientOccluder& a)
	: 	tzILight(a),
		mLs(a.mLs),
		mColor(a.mColor) ,
		mMinAmount(0.0f),
		mSamplerPtr(nullptr)
{}


//===================================================================================
std::shared_ptr<tzILight> tzAmbientOccluder::clone(void) const 
{
	return (std::make_shared< tzAmbientOccluder >(*this));
}	


//===================================================================================
tzAmbientOccluder& tzAmbientOccluder::operator= (const tzAmbientOccluder& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	mLs 	= rhs.mLs;
	mColor 	= rhs.mColor;
	
	return (*this);
}


//===================================================================================
tzAmbientOccluder::~tzAmbientOccluder(void) 
{
}


//===================================================================================
tzVector3D tzAmbientOccluder::getDirection(  tzShadeRec& s)
{
	tzPoint3D sp = mSamplerPtr->sampleHemisphere(s.mRay);
	return ( sp.x*mU + sp.y*mV + sp.z*mW );
}


//===================================================================================
tzColor tzAmbientOccluder::L( tzShadeRec& sr) 
{
	mW = sr.mNormal;
	// jitter up vector in case normal is vertical
	mV = mW^tzVector3D(0.0072f, 1.0f, 0.0034f);
	mV.normalize();
	mU = mV^mW;

	tzRay shadowRay;
	shadowRay.mOrigin = sr.mHitPoint;
	shadowRay.mDirection = getDirection( sr );

	if (inShadow( shadowRay, sr ) )
	{
		return (mMinAmount * mLs * mColor);
	}
	
	return (mLs*mColor);
}

//===================================================================================
bool tzAmbientOccluder::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t;
	int numObjects = (int)sr.mWorld.mObjects.size();

	for ( int j = 0; j < numObjects; j++ )
	{
		if ( sr.mWorld.mObjects[j]->shadowHit( ray, sr, t ) )
		{
			return true;
		}
	}

	return false;
}

// ================================================================================
void tzAmbientOccluder::setSampler(std::shared_ptr< tzISampler > samplerPtr )
{
	mSamplerPtr = samplerPtr;
	mSamplerPtr->mapSamplesToHemisphere( 1 );
}

// ================================================================================
void tzAmbientOccluder::setMinAmount(float minAmount)
{
	mMinAmount = minAmount;
}


