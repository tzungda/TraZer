#include "../include/tzISampler.h"
#include "../include/tzViewPlane.h"
#include "../include/tzMultiJittered.h"
#include "../include/tzRegular.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzViewPlane::tzViewPlane()
{
	mHeight = mWidth = 128;
	mS = 1.0f;
	mGamma = mInvGamma = 1.0f;

	mSamplerPtr = NULL;
	mNumSamples = 0;
	mMaxDepth = kHugeValue;
}

//===================================================================================
tzViewPlane::~tzViewPlane()
{
}


/*
Interfaces
*/

//===================================================================================
void tzViewPlane::setHeight(int hres)
{
	mHeight = hres;
}

//===================================================================================
void tzViewPlane::setWidth(int vres)
{
	mWidth = vres;
}

//===================================================================================
void tzViewPlane::setPixelSize(float s)
{
	mS = s;
}

//===================================================================================
void tzViewPlane::setGamma(float gamma)
{
	mGamma = gamma;
	mInvGamma = 1.0f/ gamma;
}

//===================================================================================
void tzViewPlane::setSampler(tzISampler* sp)
{
	if ( mSamplerPtr )
	{
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}

	mNumSamples = sp->getNumSamples();
	mSamplerPtr = sp;
}

//===================================================================================
void tzViewPlane::setSamples(const int n)
{
	mNumSamples = n;

	if ( mSamplerPtr )
	{
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}

	if (mNumSamples > 1)
	{
		mSamplerPtr = new tzMultiJittered( mNumSamples );	
	}
	else
	{
		mSamplerPtr = new tzRegular( 1 );
	}
}

//===================================================================================
void tzViewPlane::setMaxDepth(float maxDepth)
{
	mMaxDepth = maxDepth;
}
