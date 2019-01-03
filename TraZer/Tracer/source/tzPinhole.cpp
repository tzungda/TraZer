
// This file contains the definition of the Pinhole class

#include "../include/tzConstants.h" 
#include "tzPoint3D.h"
#include "tzVector3D.h"
#include "../include/tzPinhole.h"
#include "../include/tzMultiJittered.h"
#include <math.h>

// ----------------------------------------------------------------------------- default constructor

tzPinhole::tzPinhole(void)
	:	tzICamera(),
		d(500),
		zoom(1.0)
{}


// ----------------------------------------------------------------------------- copy constructor

tzPinhole::tzPinhole(const tzPinhole& c)
	: 	tzICamera(c),
		d(c.d),
		zoom(c.zoom)
{}


// ----------------------------------------------------------------------------- clone

tzICamera* 
tzPinhole::clone(void) const {
	return (new tzPinhole(*this));
}



// ----------------------------------------------------------------------------- assignment operator

tzPinhole&
tzPinhole::operator= (const tzPinhole& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzICamera::operator= (rhs);

	d 		= rhs.d;
	zoom	= rhs.zoom;

	return (*this);
}


// ----------------------------------------------------------------------------- destructor

tzPinhole::~tzPinhole(void) {}


// ----------------------------------------------------------------------------- get_direction

tzVector3D
tzPinhole::get_direction(const tzPoint2D& p) const {
	tzVector3D dir = p.x * u + p.y * v - d * w;
	dir.normalize();
	
	return(dir);
}



// ----------------------------------------------------------------------------- render_scene

void 												
tzPinhole::renderScene(const tzWorld& w) {
	tzRGBColor	L;
	tzViewPlane	vp(w.mVp);	 								
	tzRay			ray;
	int 		depth = 0;  
	tzPoint2D 	pp;		// sample point on a pixel
	int n = (int)sqrt((float)vp.mNumSamples);
		
	vp.mS /= zoom;
	ray.o = eye;

	std::vector<glm::vec4> colorBuffer;
	colorBuffer.resize(vp.mVres*vp.mHres);

	float invNumSamples = 1.0f/(float)vp.mNumSamples;
		
	for (int r = 0; r < vp.mVres; r++)			// up
	{
		for (int c = 0; c < vp.mVres; c++) {		// across 					
			L = black; 
			
			for (int p = 0; p < n; p++)			// up pixel
				for (int q = 0; q < n; q++) {	// across pixel
					pp.x = vp.mS * (c - 0.5f * vp.mHres + (q + 0.5f) / n); 
					pp.y = vp.mS * (r - 0.5f * vp.mVres + (p + 0.5f) / n);
					ray.d = get_direction(pp);
					L += w.mTracerPtr->trace_ray(ray, depth);
				}	
											
			L *= invNumSamples;
			L *= exposure_time;

			// test
			if ( L.r > 1.0f )
			{
				L.r = 1.0f;
			}
			if ( L.g > 1.0f )
			{
				L.g = 1.0f;
			}
			if ( L.b > 1.0f )
			{
				L.b = 1.0f;
			}
			// 

			w.writeToBuffer(colorBuffer, r, c, L);
		} 
	}

	// output png
	//std::string outPath("C:\\Users\\User\\Desktop\\TraZer\\TraZer\\testImages\\areaLight.png");
	w.writeImage(colorBuffer, mOutputPath);
}



