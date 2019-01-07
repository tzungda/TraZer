
#include "../include/tzSphericalMap.h"

// ---------------------------------------------------------------- default constructor

tzSphericalMap::tzSphericalMap(void) {}


// ---------------------------------------------------------------- copy constructor

tzSphericalMap::tzSphericalMap(const tzSphericalMap& sm) {}
	
	
// ---------------------------------------------------------------- assignment operator

tzSphericalMap&
tzSphericalMap::operator= (const tzSphericalMap& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzSphericalMap::~tzSphericalMap(void) {}


// ---------------------------------------------------------------- clone

tzSphericalMap*
tzSphericalMap::clone(void) const {
	return (new tzSphericalMap(*this));
}	


// ---------------------------------------------------------------- get_texel_coordinates

// Given a hit point on a generic sphere, and the image resolution, this function 
// returns the texel coordinates in the image

void
tzSphericalMap::get_texel_coordinates(	const 	tzPoint3D& 	local_hit_point,
										const 	int 		xres, 
										const 	int 		yres, 
												int& 		row, 
												int& 		column) const {
	
	// first, compute theta and phi
	
	float theta = (float)acos(local_hit_point.y);
	float phi   = (float)atan2(local_hit_point.x, local_hit_point.z);
	if (phi < 0.0)
		phi += (float)TWO_PI;
	
	// next, map theta and phi to (u, v) in [0, 1] X [0, 1]
		
	float u = (float)phi * invTWO_PI;    
	float v = 1.0f - (float)theta * invPI;           
			
	// finally, map u and v to the texel coordinates
		
	column 	= (int) ((xres - 1) * u);   	// column is across
	row 	= (int)  ((yres - 1) * v);    	// row is up
}
