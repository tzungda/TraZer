
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


// -------------------------------------------------------------------- trace_ray

tzRGBColor	
tzPathTrace::trace_ray(const tzRay ray, const int depth) const {
	float tmin = 0.0f;
	if (depth > world_ptr->mVp.mMaxDepth)
		return (black);
	else {
		tzShadeRec sr(world_ptr->hitObjects(ray, tmin));
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;
			
			return (sr.mMaterialPtr->path_shade(sr));   
		}
		else
			return (world_ptr->mBackgroundColor);
	}	
}


// -------------------------------------------------------------------- trace_ray
// this version has tmin as an argument, and is used only with the Dielectric material
// for color filtering

tzRGBColor	
tzPathTrace::trace_ray(const tzRay ray, double& tmin, const int depth) const {
	float t = 0.0f;
	if (depth > world_ptr->mVp.mMaxDepth)
		return (black);
	else {
		tzShadeRec sr(world_ptr->hitObjects(ray, t));  
					
		if (sr.mHitAnObject) {
			sr.mDepth 	= depth;
			sr.mRay 		= ray;
			tmin		= sr.mT;     // required for colored transparency
			return (sr.mMaterialPtr->path_shade(sr));   
		}
		else{
			tmin = kHugeValue;
			return (world_ptr->mBackgroundColor);
		}
	}	
}
