#include "tzRay.h"
#include "../include/tzConstants.h"

// ================================================================================
tzRay::~tzRay(void) 
{
}

// ================================================================================
tzRay::tzRay(void)
	: mOrigin(0.0),
	mDirection(0.0, 0.0, 1.0),
	mDepth(0)
{
	mThreadId = 0;
	mMaxThreads = 1;
}

// ================================================================================
tzRay::tzRay(const tzPoint3D& origin, const tzVector3D& dir)
	: mOrigin(origin), mDirection(dir), mDepth(0)
{
	mThreadId = 0;
	mMaxThreads = 1;
}

// ================================================================================
tzRay::tzRay(const tzRay& ray)
	: mOrigin(ray.mOrigin),
	mDirection(ray.mDirection),
	mDepth(ray.mDepth)
{
	mThreadId = ray.mThreadId;
	mMaxThreads = ray.mMaxThreads;
}

// ================================================================================
tzRay&
tzRay::operator= (const tzRay& rhs) {

	if (this == &rhs)
		return (*this);

	mOrigin = rhs.mOrigin;
	mDirection = rhs.mDirection;
	mDepth = rhs.mDepth;

	mThreadId = rhs.mThreadId;
	mMaxThreads = rhs.mMaxThreads;

	return (*this);
}

