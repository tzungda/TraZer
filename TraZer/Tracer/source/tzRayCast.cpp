
#include "../include/tzRayCast.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"
#include "../include/tzIMaterial.h"

//===================================================================================
tzRayCast::tzRayCast(void)
	: tzITracer()
{}


//===================================================================================		
tzRayCast::tzRayCast(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


//===================================================================================
tzRayCast::~tzRayCast(void) {}


//===================================================================================
tzColor tzRayCast::traceRay(const tzRay ray, const int depth) const 
{
	float tmin = 0.0f;
	tzShadeRec sr(mWorldPtr->hitObjects(ray, tmin));
		
	if (sr.mHitAnObject) {
		sr.mRay = ray;			// used for specular shading
		return (sr.mMaterialPtr->shade(sr));
	}   
	else
		return (mWorldPtr->mBackgroundColor);
}


