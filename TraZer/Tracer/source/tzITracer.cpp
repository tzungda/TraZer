#include "../include/tzWorld.h"
#include "../include/tzITracer.h"

//===================================================================================
tzITracer::tzITracer(tzWorld& _worldPtr)
	: mWorldPtr(_worldPtr)
{}

//===================================================================================
tzITracer::~tzITracer(void) 
{
}

//===================================================================================
tzColor tzITracer::traceRay(const tzRay& ray) const 
{
	return black;
}

//===================================================================================
tzColor tzITracer::traceRay(const tzRay ray, const int depth) const 
{
	return black;
}

