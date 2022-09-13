
#include "../include/tzSphericalMap.h"

//===================================================================================
tzSphericalMap::tzSphericalMap(void) {}

//===================================================================================
tzSphericalMap::tzSphericalMap(const tzSphericalMap& sm) 
{}
	
//===================================================================================
tzSphericalMap& tzSphericalMap::operator= (const tzSphericalMap& rhs) 
{
	if (this == &rhs)
		return (*this);

	return (*this);
}

//===================================================================================
tzSphericalMap::~tzSphericalMap(void) {}

//===================================================================================
std::shared_ptr<tzIMapping> tzSphericalMap::clone(void) const 
{
	return (std::make_shared< tzSphericalMap >(*this));
}	

//===================================================================================
void tzSphericalMap::getTexelCoordinates(	const 	tzPoint3D& 	local_hit_point,
										const 	int 		xres, 
										const 	int 		yres, 
												int& 		row, 
												int& 		column) const 
{
	
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
