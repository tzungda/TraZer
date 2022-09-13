
// This file contains the definition of the Pinhole class

#include "../include/tzConstants.h" 
#include "tzPoint3D.h"
#include "tzVector3D.h"
#include "../include/tzPinhole.h"
#include "../include/tzMultiJittered.h"
#include <math.h>
#include <time.h>

#ifdef _OPENMP 
#include <omp.h>
#endif

//===================================================================================
tzPinhole::tzPinhole(void)
	:	tzICamera(),
		mDistance(500.0f),
		mZoom(1.0f)
{
}

//===================================================================================
tzPinhole::tzPinhole(const tzPinhole& c)
	: 	tzICamera(c),
		mDistance(c.mDistance),
		mZoom(c.mZoom)
{
}

//===================================================================================
std::shared_ptr<tzICamera> tzPinhole::clone(void) const 
{
	return (std::make_shared< tzPinhole >(*this));
}

//===================================================================================
tzPinhole& tzPinhole::operator= (const tzPinhole& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzICamera::operator= (rhs);

	mDistance = rhs.mDistance;
	mZoom = rhs.mZoom;

	return (*this);
}

//===================================================================================
tzPinhole::~tzPinhole(void) 
{
}

//===================================================================================
tzVector3D tzPinhole::getDirection(const tzPoint2D& p) const
{
	tzVector3D dir = p.x * mOrthoU + p.y * mOrthoV - mDistance * mOrthoW;
	dir.normalize();
	
	return(dir);
}

//===================================================================================
void tzPinhole::renderScene(const tzWorld& w) const
{
	tzViewPlane	vp(w.mVp);
	const int n = (int)sqrt((float)vp.mNumSamples);

	vp.mS /= mZoom;
	

	std::vector<tzColor> colorBuffer;
	colorBuffer.resize(vp.mWidth*vp.mHeight);

	const float invNumSamples = 1.0f/(float)vp.mNumSamples;
	const float invN = 1.0f / (float)n;
		
	const float halfH = 0.5f * vp.mHeight;
	const float halfW = 0.5f * vp.mWidth;

	for (int r = 0; r < vp.mHeight; r++)
	{
#ifdef _OPENMP
		int maxThreads = omp_get_max_threads();
		omp_set_num_threads(maxThreads);
		#pragma omp parallel for
#endif
		for (int c = 0; c < vp.mWidth; c++) 
		{
			tzColor	threadL( black );
			int 		depth = 0;
			tzPoint2D 	pp;
			tzRay ray;
			ray.mOrigin = mEye;
			ray.mMaxThreads = maxThreads;
			ray.mThreadId = omp_get_thread_num();

			for (int p = 0; p < n; p++)			// up pixel
			{
				for (int q = 0; q < n; q++) // across pixel
				{	
					pp.x = vp.mS * (c - halfH + (q + 0.5f)*invN);
					pp.y = vp.mS * (r - halfW + (p + 0.5f)*invN);
					ray.mDirection = getDirection(pp);
					threadL += w.mTracerPtr->traceRay(ray, depth);
				}
			}

			threadL *= invNumSamples;
			threadL *= mExposureTime;

			// clamp color
			if (threadL.r > 1.0f)
			{
				threadL.r = 1.0f;
			}
			if (threadL.g > 1.0f)
			{
				threadL.g = 1.0f;
			}
			if (threadL.b > 1.0f)
			{
				threadL.b = 1.0f;
			}
			// 
			w.writeToBuffer(colorBuffer, r, c, threadL);
		} 
	}

	// output 
	w.writeImage(colorBuffer, mOutputPath);
}



