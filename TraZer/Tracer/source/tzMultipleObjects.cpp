
#include "../include/tzMultipleObjects.h"
#include "../include/tzWorld.h"


//===================================================================================
tzMultipleObjects::tzMultipleObjects (tzWorld& _worldPtr)
	: tzITracer(_worldPtr)
{}

//===================================================================================
tzMultipleObjects::~tzMultipleObjects(void) 
{}

//===================================================================================
tzColor tzMultipleObjects::traceRay(const tzRay& ray) const 
{
	tzShadeRec sr(mWorldPtr.hitBareBonesObject(ray)); 
		
	if (sr.mHitAnObject)   
		return (sr.mColor);   
	else
		return (mWorldPtr.mBackgroundColor);
}

//===================================================================================
tzColor tzMultipleObjects::traceRay(const tzRay ray, const int depth) const 
{
	if (depth > mWorldPtr.mVp.mMaxDepth)
		return (black);

	tzShadeRec sr(mWorldPtr.hitBareBonesObject(ray));

	if (sr.mHitAnObject)
		return (sr.mColor);
	else
		return (mWorldPtr.mBackgroundColor);
}


