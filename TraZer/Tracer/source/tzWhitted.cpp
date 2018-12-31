
#include "../include/tzWhitted.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"

// -------------------------------------------------------------------- default constructor

tzWhitted::tzWhitted(void)
	: tzITracer()
{}


// -------------------------------------------------------------------- constructor
		
tzWhitted::tzWhitted(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

tzWhitted::~tzWhitted(void) {}


// -------------------------------------------------------------------- trace_ray

tzRGBColor	
tzWhitted::trace_ray(const tzRay ray, const int depth) const {
	float tmin = 0.0f;
	if (depth > world_ptr->mVp.mMaxDepth)
		return(black);
	else {
		tzShadeRec sr(world_ptr->hitObjects(ray, tmin));
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;	
			return (sr.mMaterialPtr->shade(sr));   
		}
		else
			return (world_ptr->mBackgroundColor);
	}																																			
}
