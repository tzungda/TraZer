
#include "../include/tzRayCast.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"

// -------------------------------------------------------------------- default constructor

tzRayCast::tzRayCast(void)
	: tzITracer()
{}


// -------------------------------------------------------------------- constructor
		
tzRayCast::tzRayCast(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

tzRayCast::~tzRayCast(void) {}


// -------------------------------------------------------------------- trace_ray
// this ignores the depth argument

tzRGBColor
tzRayCast::trace_ray(const tzRay ray, const int depth) const {
	float tmin = 0.0f;
	tzShadeRec sr(world_ptr->hitObjects(ray, tmin));
		
	if (sr.mHitAnObject) {
		sr.mRay = ray;			// used for specular shading
		return (sr.mMaterialPtr->shade(sr));
	}   
	else
		return (world_ptr->mBackgroundColor);
}


