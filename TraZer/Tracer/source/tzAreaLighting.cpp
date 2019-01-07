
#include "../include/tzAreaLighting.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"

// -------------------------------------------------------------------- default constructor

tzAreaLighting::tzAreaLighting(void)
	: tzITracer()
{}


// -------------------------------------------------------------------- constructor
		
tzAreaLighting::tzAreaLighting(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

tzAreaLighting::~tzAreaLighting(void) {}


// -------------------------------------------------------------------- trace_ray

// The statement sr.ray = ray; is required for specular shading, computing the reflected and 
// transmitted ray directions, and for rendering single sided emissive materials with area lights

tzRGBColor tzAreaLighting::trace_ray(const tzRay ray, const int depth) const {
	if (depth > world_ptr->mVp.mMaxDepth)
		return (black);
	else {
		float tmin;
		tzShadeRec sr(world_ptr->hitObjects(ray, tmin));   
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;	
			
			return (sr.mMaterialPtr->area_light_shade(sr));   
		}
		else
			return (world_ptr->mBackgroundColor);
	}																																			
}


