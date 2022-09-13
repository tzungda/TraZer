
#include "../include/tzWhitted.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"


//===================================================================================		
tzWhitted::tzWhitted(tzWorld& _worldPtr)
	: tzITracer(_worldPtr)
{}


//===================================================================================
tzWhitted::~tzWhitted(void) {}


//===================================================================================
tzColor tzWhitted::traceRay(const tzRay ray, const int depth) const
{
	float tmin = 0.0f;
	if (depth > mWorldPtr.mVp.mMaxDepth)
		return(black);
	else {
		tzShadeRec sr(mWorldPtr.hitObjects(ray, tmin));
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;	
			return (sr.mMaterialPtr[ray.mThreadId]->shade(sr));   
		}
		else
			return (mWorldPtr.mBackgroundColor);
	}																																			
}
