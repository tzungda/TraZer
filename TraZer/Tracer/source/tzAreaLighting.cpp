
#include "../include/tzAreaLighting.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"

//===================================================================================
tzAreaLighting::tzAreaLighting(void)
	: tzITracer()
{}


//===================================================================================		
tzAreaLighting::tzAreaLighting(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


//===================================================================================
tzAreaLighting::~tzAreaLighting(void) {}


//===================================================================================
tzRGBColor tzAreaLighting::traceRay(const tzRay ray, const int depth) const 
{
	if (depth > mWorldPtr->mVp.mMaxDepth)
		return (black);
	else {
		float tmin;
		tzShadeRec sr(mWorldPtr->hitObjects(ray, tmin));   
					
		if (sr.mHitAnObject) {
			sr.mDepth = depth;
			sr.mRay = ray;	
			
			return (sr.mMaterialPtr->areaLightShade(sr));   
		}
		else
			return (mWorldPtr->mBackgroundColor);
	}																																			
}


