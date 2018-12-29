#include "../include/tzWorld.h"
#include "../include/tzITracer.h"

// -------------------------------------------------------------------- default constructor

tzITracer::tzITracer(void)
	: world_ptr(NULL)
{}


// -------------------------------------------------------------------- constructor

tzITracer::tzITracer(tzWorld* _worldPtr)
	: world_ptr(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

tzITracer::~tzITracer(void) {
	if (world_ptr)
		world_ptr = NULL;
}


// -------------------------------------------------------------------- trace_ray

RGBColor	
tzITracer::trace_ray(const tzRay& ray) const {
	return (black);
}


// -------------------------------------------------------------------- trace_ray

RGBColor	
tzITracer::trace_ray(const tzRay ray, const int depth) const {
	return (black);
}




