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


// -------------------------------------------------------------------- trace_ray

tzRGBColor
tzSingleSphere::trace_ray(const tzRay& ray) const {
	tzShadeRec	sr(*world_ptr); 	// not used
	double    	t;  				// not used
	
	if (world_ptr->mSphere.hit(ray, t, sr))		
		return (red);  
	else
		return (black);   
}



