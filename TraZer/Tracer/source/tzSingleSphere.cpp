
#include "../include/tzSingleSphere.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"

//===================================================================================
tzSingleSphere::tzSingleSphere(tzWorld& _worldPtr)
	: tzITracer(_worldPtr)
{}

//===================================================================================
tzSingleSphere::~tzSingleSphere(void) 
{
}

//===================================================================================
tzColor tzSingleSphere::traceRay(const tzRay& ray) const 
{
	tzShadeRec	sr(mWorldPtr); 	// not used
	float    	t;  				// not used
	
	if (mWorldPtr.mSphere.hit(ray, t, sr))		
		return (red);  
	else
		return (black);   
}



