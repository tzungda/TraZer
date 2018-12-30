
#include "../include/tzMultipleObjects.h"
#include "../include/tzWorld.h"

// -------------------------------------------------------------------- default constructor

tzMultipleObjects::tzMultipleObjects(void)
	: tzITracer()
{}


// -------------------------------------------------------------------- constructor
		
tzMultipleObjects::tzMultipleObjects (tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

tzMultipleObjects::~tzMultipleObjects(void) {}


// -------------------------------------------------------------------- trace_ray

tzRGBColor
tzMultipleObjects::trace_ray(const tzRay& ray) const {
	tzShadeRec sr(world_ptr->hitBareBonesObject(ray)); 
		
	if (sr.mHitAnObject)   
		return (sr.mColor);   
	else
		return (world_ptr->mBackgroundColor);
}

tzRGBColor
tzMultipleObjects::trace_ray(const tzRay ray, const int depth) const {
	if (depth > world_ptr->mVp.mMaxDepth)
		return (black);

	tzShadeRec sr(world_ptr->hitBareBonesObject(ray));

	if (sr.mHitAnObject)
		return (sr.mColor);
	else
		return (world_ptr->mBackgroundColor);
}


