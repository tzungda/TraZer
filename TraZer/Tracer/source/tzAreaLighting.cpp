
#include "../include/tzAreaLighting.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"


//===================================================================================		
tzAreaLighting::tzAreaLighting(tzWorld& _worldPtr)
	: tzITracer(_worldPtr)
{}


//===================================================================================
tzAreaLighting::~tzAreaLighting(void) {}


//===================================================================================
tzColor tzAreaLighting::traceRay(const tzRay ray, const int depth) const 
{
	if (depth > mWorldPtr.mVp.mMaxDepth)
		return (black);
	else {
		float tmin;
		tzShadeRec sr(mWorldPtr.hitObjects(ray, tmin));   
		sr.mThreadId = ray.mThreadId;
		sr.mMaxThreads = ray.mMaxThreads;
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;	
			
			return (sr.mMaterialPtr[ray.mThreadId]->areaLightShade(sr));   
			//return sr.mMaterialPtr->shade( sr );
		}
		else
			return (mWorldPtr.mBackgroundColor);
	}																																			
}


