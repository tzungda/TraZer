
#include "tzBBox.h"
#include "../Include/tzConstants.h"


// ================================================================================
tzBBox::tzBBox (void)
	: x0(-1), x1(1), y0(-1), y1(1), z0(-1), z1(1)
{}	


// ================================================================================
tzBBox::tzBBox (	const float _x0, const float _x1,			
				const float _y0, const float _y1, 
				const float _z0, const float _z1)
	: x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1)
{}


// ================================================================================
tzBBox::tzBBox(const tzPoint3D p0, const tzPoint3D p1)
	: x0(p0.x), x1(p1.x), y0(p0.y), y1(p1.y), z0(p0.z), z1(p1.z)
{}
										


// ================================================================================
tzBBox::tzBBox(const tzBBox& bbox)
	: x0(bbox.x0), x1(bbox.x1), y0(bbox.y0), y1(bbox.y1), z0(bbox.z0), z1(bbox.z1)
{}						
				

// ================================================================================
tzBBox& tzBBox::operator= (const tzBBox& rhs) 
{
	if (this == &rhs)
		return (*this);

	x0	= rhs.x0;
	x1	= rhs.x1;
	y0	= rhs.y0;
	y1	= rhs.y1;
	z0	= rhs.z0;
	z1	= rhs.z1;	
	
	return (*this);
}			


// ================================================================================
tzBBox::~tzBBox(void) 
{
}


// ================================================================================
bool tzBBox::hit(const tzRay& ray) const 
{
	float ox = ray.o.x; float oy = ray.o.y; float oz = ray.o.z;
	float dx = ray.d.x; float dy = ray.d.y; float dz = ray.d.z;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max; 

	float a = 1.0f / dx;
	if (a >= 0.0f) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	float b = 1.0f / dy;
	if (b >= 0.0f) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	float t0, t1;
	
	// find largest entering t value
	
	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;
		
	if (tz_min > t0)
		t0 = tz_min;	
		
	// find smallest exiting t value
		
	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;
		
	if (tz_max < t1)
		t1 = tz_max;
		
	return (t0 < t1 && t1 > kEpsilon);
}

// ================================================================================
bool tzBBox::inside(const tzPoint3D& p) const 
{
	return ((p.x > x0 && p.x < x1) && (p.y > y0 && p.y < y1) && (p.z > z0 && p.z < z1));
};




