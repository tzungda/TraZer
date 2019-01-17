// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "../include/tzSingleSphere.h"
#include "../include/tzWorld.h"
#include "../include/tzShadeRec.h"

// -------------------------------------------------------------------- default constructor

tzSingleSphere::tzSingleSphere(void)
	: tzITracer()
{}


// -------------------------------------------------------------------- constructor
		
tzSingleSphere::tzSingleSphere(tzWorld* _worldPtr)
	: tzITracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

tzSingleSphere::~tzSingleSphere(void) {}


// -------------------------------------------------------------------- traceRay

tzColor
tzSingleSphere::traceRay(const tzRay& ray) const {
	tzShadeRec	sr(*mWorldPtr); 	// not used
	float    	t;  				// not used
	
	if (mWorldPtr->mSphere.hit(ray, t, sr))		
		return (red);  
	else
		return (black);   
}



