
#include "../include/tzWorld.h"
#include "../include/tzAmbientOccluder.h"

//===================================================================================
tzAmbientOccluder::tzAmbientOccluder(void)
	: 	tzILight(),
		mLs(1.0),
		mColor(1.0),
		mMinAmount( 0.0f ),
		mSamplerPtr( NULL )
{}


//===================================================================================
tzAmbientOccluder::tzAmbientOccluder(const tzAmbientOccluder& a)
	: 	tzILight(a),
		mLs(a.mLs),
		mColor(a.mColor) ,
		mMinAmount(0.0f),
		mSamplerPtr(NULL)
{}


//===================================================================================
tzILight* tzAmbientOccluder::clone(void) const 
{
	return (new tzAmbientOccluder(*this));
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
	if ( mSamplerPtr )
	{
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}
}


//===================================================================================
tzVector3D tzAmbientOccluder::getDirection(  tzShadeRec& s)
{
	tzPoint3D sp = mSamplerPtr->sampleHemisphere();
	return ( sp.x*u + sp.y*v + sp.z*w );
}


//===================================================================================
tzRGBColor tzAmbientOccluder::L( tzShadeRec& sr) 
{
	w = sr.mNormal;
	// jitter up vector in case normal is vertical
	v = w^tzVector3D(0.0072f, 1.0f, 0.0034f);
	v.normalize();
	u = v^w;

	tzRay shadowRay;
	shadowRay.o = sr.mHitPoint;
	shadowRay.d = getDirection( sr );

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
		if ( sr.mWorld.mObjects[j]->shadowHit( ray, t ) )
		{
			return true;
		}
	}

	return false;
}

// ================================================================================
void tzAmbientOccluder::setSampler( tzISampler *samplerPtr )
{
	if ( mSamplerPtr )
	{
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}

	mSamplerPtr = samplerPtr;
	mSamplerPtr->mapSamplesToHemisphere( 1 );
}

// ================================================================================
void tzAmbientOccluder::setMinAmount(float minAmount)
{
	mMinAmount = minAmount;
}


