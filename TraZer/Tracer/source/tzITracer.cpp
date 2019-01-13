#include "../include/tzWorld.h"
#include "../include/tzITracer.h"

// -------------------------------------------------------------------- default constructor

tzITracer::tzITracer(void)
	: mWorldPtr(NULL)
{}


// -------------------------------------------------------------------- constructor

tzITracer::tzITracer(tzWorld* _worldPtr)
	: mWorldPtr(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

tzITracer::~tzITracer(void) {
	if (mWorldPtr)
		mWorldPtr = NULL;
}


// -------------------------------------------------------------------- traceRay

tzRGBColor
tzITracer::traceRay(const tzRay& ray) const {
	return (black);
}


// -------------------------------------------------------------------- traceRay

tzRGBColor
tzITracer::traceRay(const tzRay ray, const int depth) const {
	return (black);
}

