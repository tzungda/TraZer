
#include "../include/tzRectangle.h"

const float tzRectangle::kEpsilon = 0.001f;

// ----------------------------------------------------------------  default constructor

tzRectangle::tzRectangle(void)
	: 	tzIGeometricObject(),
		p0(-1, 0, -1), 
		a(0, 0, 2), b(2, 0, 0), 
		a_len_squared(4.0), 
		b_len_squared(4.0),
		normal(0, 1, 0),
		area(4.0),
		inv_area(0.25),
		sampler_ptr(NULL)
{}


// ----------------------------------------------------------------  constructor
// this constructs the normal

tzRectangle::tzRectangle(const tzPoint3D& _p0, const tzVector3D& _a, const tzVector3D& _b)
	:	tzIGeometricObject(),
		p0(_p0),
		a(_a),
		b(_b),
		a_len_squared((float)(a.len_squared())), 
		b_len_squared((float)(b.len_squared())),
		area((float)(a.length() * b.length())),
		inv_area(1.0f / (float)area),
		sampler_ptr(NULL)		
{
	normal = a ^ b;
	normal.normalize();
}


// ----------------------------------------------------------------  constructor
// this has the normal as an argument

tzRectangle::tzRectangle(const tzPoint3D& _p0, const tzVector3D& _a, const tzVector3D& _b, const tzNormal& n)
	:	tzIGeometricObject(),
		p0(_p0),
		a(_a),
		b(_b),
		a_len_squared(a.len_squared()), 
		b_len_squared(b.len_squared()),
		area((float)(a.length() * b.length())),	
		inv_area(1.0f / (float)area),
		normal(n),
		sampler_ptr(NULL)
{
	normal.normalize();
}



// ---------------------------------------------------------------- clone

tzRectangle*
tzRectangle::clone(void) const {
	return (new tzRectangle(*this));
}


// ---------------------------------------------------------------- copy constructor

tzRectangle::tzRectangle(const tzRectangle& r)
	:	tzIGeometricObject(r),
		p0(r.p0), 
		a(r.a),
		b(r.b),
		a_len_squared(r.a_len_squared), 
		b_len_squared(r.b_len_squared),	
		normal(r.normal),
		area(r.area),
		inv_area(r.inv_area)
{
	if(r.sampler_ptr)
		sampler_ptr	= r.sampler_ptr->clone(); 
	else  sampler_ptr = NULL;
}



// ---------------------------------------------------------------- assignment operator

tzRectangle&
tzRectangle::operator= (const tzRectangle& rhs) {
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator=(rhs);
	
	p0				= rhs.p0;
	a				= rhs.a;
	b				= rhs.b;
	a_len_squared	= rhs.a_len_squared; 
	b_len_squared	= rhs.b_len_squared;
	area			= rhs.area;	
	inv_area		= rhs.inv_area;
	normal			= rhs.normal;
	
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	if (rhs.sampler_ptr)
		sampler_ptr= rhs.sampler_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzRectangle::~tzRectangle(void) {

	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
}

//------------------------------------------------------------------ getBoundingBox 

tzBBox
tzRectangle::getBoundingBox(void) {
	float delta = 0.0001f;

	return(tzBBox(fmin(p0.x, p0.x + a.x + b.x) - delta, fmax(p0.x, p0.x + a.x + b.x) + delta,
				fmin(p0.y, p0.y + a.y + b.y) - delta, fmax(p0.y, p0.y + a.y + b.y) + delta, 
				fmin(p0.z, p0.z + a.z + b.z) - delta, fmax(p0.z, p0.z + a.z + b.z) + delta));
}
																			

//------------------------------------------------------------------ hit 

bool 												 
tzRectangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const {
	
	float t = (p0 - ray.o) * normal / (ray.d * normal);
	
	if (t <= kEpsilon)
		return (false);
			
	tzPoint3D p = ray.o + t * ray.d;
	tzVector3D d = p - p0;
	
	float ddota = d * a;
	
	if (ddota < 0.0 || ddota > a_len_squared)
		return (false);
		
	float ddotb = d * b;
	
	if (ddotb < 0.0 || ddotb > b_len_squared)
		return (false);
		
	tmin 				= t;
	sr.mNormal 			= normal;
	sr.mLocalHitPoint 	= p;
	
	return (true);
}


// ---------------------------------------------------------------- setSampler

void 								
tzRectangle::set_sampler(tzISampler* sampler) {
	sampler_ptr = sampler;
}


// ---------------------------------------------------------------- sample
// returns a sample point on the rectangle

tzPoint3D
tzRectangle::sample(void) {
	tzPoint2D sample_point = sampler_ptr->sampleUnitSquare();
	return (p0 + sample_point.x * a + sample_point.y * b);
}


//------------------------------------------------------------------ getNormal 
					 
tzNormal 										
tzRectangle::getNormal(const tzPoint3D& p) {
	return (normal);
}


// ---------------------------------------------------------------- pdf

float
tzRectangle::pdf(tzShadeRec& sr) {
	return (inv_area);
} 





