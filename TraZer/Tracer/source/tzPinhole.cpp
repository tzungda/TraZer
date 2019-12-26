
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


// ----------------------------------------------------------------------------- default constructor

tzPinhole::tzPinhole(void)
	:	tzICamera(),
	mDistance(500.0f),
		mZoom(1.0f)
{
}


// ----------------------------------------------------------------------------- copy constructor

tzPinhole::tzPinhole(const tzPinhole& c)
	: 	tzICamera(c),
	mDistance(c.mDistance),
		mZoom(c.mZoom)
{
}


// ----------------------------------------------------------------------------- clone

tzICamera* tzPinhole::clone(void) const 
{
	return (new tzPinhole(*this));
}



// ----------------------------------------------------------------------------- assignment operator

tzPinhole& tzPinhole::operator= (const tzPinhole& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	tzICamera::operator= (rhs);

	mDistance = rhs.mDistance;
	mZoom = rhs.mZoom;

	return (*this);
}


// ----------------------------------------------------------------------------- destructor

tzPinhole::~tzPinhole(void) 
{
}


// ----------------------------------------------------------------------------- get_direction

tzVector3D tzPinhole::getDirection(const tzPoint2D& p) const
{
	tzVector3D dir = p.x * mOrthoU + p.y * mOrthoV - mDistance * mOrthoW;
	dir.normalize();
	
	return(dir);
}



// ----------------------------------------------------------------------------- render_scene

void tzPinhole::renderScene(const tzWorld& w) const
{
	tzViewPlane	vp(w.mVp);
	const int n = (int)sqrt((float)vp.mNumSamples);

	vp.mS /= mZoom;
	
#ifndef _OPENMP
	tzColor	L;
	tzRay			ray;
	int 		depth = 0;
	tzPoint2D 	pp;		// sample point on a pixel
	ray.mOrigin = eye;
#endif

	std::vector<tzColor> colorBuffer;
	colorBuffer.resize(vp.mWidth*vp.mHeight);

	const float invNumSamples = 1.0f/(float)vp.mNumSamples;
	const float invN = 1.0f / (float)n;
		
	const float halfH = 0.5f * vp.mHeight;
	const float halfW = 0.5f * vp.mWidth;

	for (int r = 0; r < vp.mHeight; r++)
	{

#ifdef _OPENMP
		//omp_lock_t writelock;
		//omp_init_lock(&writelock);
		int maxThreads = omp_get_max_threads();
		//omp_set_num_threads(1);
		//int threadnum = omp_get_thread_num();
		omp_set_num_threads(maxThreads);
		#pragma omp parallel for
#endif

		for (int c = 0; c < vp.mWidth; c++) 
		{
#ifndef _OPENMP
			L = black; 
#else
			//int threadnumX = omp_get_thread_num();
			tzColor	threadL( black );
			int 		depth = 0;
			tzPoint2D 	pp;
			tzRay ray;
			ray.mOrigin = mEye;
			ray.mMaxThreads = maxThreads;
			ray.mThreadId = omp_get_thread_num();
#endif

//----------------------------------------------------------------------
#ifndef _OPENMP			
			for (int p = 0; p < n; p++)			// up pixel
				for (int q = 0; q < n; q++) {	// across pixel
					pp.x = vp.mS * (c - 0.5f * vp.mHeight + (q + 0.5f)*invN);
					pp.y = vp.mS * (r - 0.5f * vp.mWidth + (p + 0.5f)*invN);
					ray.d = getDirection(pp);
					L += w.mTracerPtr->traceRay(ray, depth);
				}	
											
			L *= invNumSamples;
			L *= mExposureTime;

			// clamp color
			if ( L.r > 1.0f )
			{
				L.r = 1.0f;
			}
			if ( L.g > 1.0f )
			{
				L.g = 1.0f;
			}
			if ( L.b > 1.0f )
			{
				L.b = 1.0f;
			}
			// 
			w.writeToBuffer(colorBuffer, r, c, L);
#else
			//tzRay			ray;
			
			//tzPoint2D 	pp;		// sample point on a pixel
			//ray.mOrigin = eye;

			//omp_set_lock(&writelock);//-------------------------

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

			//omp_unset_lock(&writelock);//-------------------------

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
			//omp_set_lock(&writelock);

			w.writeToBuffer(colorBuffer, r, c, threadL);

			//omp_unset_lock(&writelock);
#endif
		} 

#ifdef _OPENMP
		//omp_destroy_lock(&writelock);
#endif

	}

	// output png
	//std::string outPath("C:\\Users\\User\\Desktop\\TraZer\\TraZer\\testImages\\areaLight.png");
	w.writeImage(colorBuffer, mOutputPath);
}



