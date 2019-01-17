
#include "../include/tzPathTrace.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"

// -------------------------------------------------------------------- default constructor

tzPathTrace::tzPathTrace(void)
	: tzITracer()
{}


// -------------------------------------------------------------------- default constructor
		
tzPathTrace::tzPathTrace(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


// -------------------------------------------------------------------- traceRay

tzColor	
tzPathTrace::traceRay(const tzRay ray, const int depth) const {
	float tmin = 0.0f;
	if (depth > mWorldPtr->mVp.mMaxDepth)
		return (black);
	else {
		tzShadeRec sr(mWorldPtr->hitObjects(ray, tmin));
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;
			
			return (sr.mMaterialPtr->pathShade(sr));   
		}
		else
			return (mWorldPtr->mBackgroundColor);
	}	
}


// -------------------------------------------------------------------- traceRay
// this version has tmin as an argument, and is used only with the Dielectric material
// for color filtering

tzColor	
tzPathTrace::traceRay(const tzRay ray, float& tmin, const int depth) const {
	float t = 0.0f;
	if (depth > mWorldPtr->mVp.mMaxDepth)
		return (black);
	else {
		tzShadeRec sr(mWorldPtr->hitObjects(ray, t));  
					
		if (sr.mHitAnObject) {
			sr.mDepth 	= depth;
			sr.mRay 		= ray;
			tmin		= sr.mT;     // required for colored transparency
			return (sr.mMaterialPtr->pathShade(sr));   
		}
		else{
			tmin = kHugeValue;
			return (mWorldPtr->mBackgroundColor);
		}
	}	
}
