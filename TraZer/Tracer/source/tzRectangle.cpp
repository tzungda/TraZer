
#include "../include/tzRectangle.h"

const float tzRectangle::kEpsilon = 0.001f;

//===================================================================================
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


//===================================================================================
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

//===================================================================================
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


//===================================================================================
tzRectangle* tzRectangle::clone(void) const
{
	return (new tzRectangle(*this));
}

//===================================================================================
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


//===================================================================================
tzRectangle& tzRectangle::operator= (const tzRectangle& rhs)
{
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

//===================================================================================
tzRectangle::~tzRectangle(void)
{

	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
}

//===================================================================================
tzBBox tzRectangle::getBoundingBox(void) 
{
	float delta = 0.0001f;

	return(tzBBox(fmin(p0.x, p0.x + a.x + b.x) - delta, fmax(p0.x, p0.x + a.x + b.x) + delta,
				fmin(p0.y, p0.y + a.y + b.y) - delta, fmax(p0.y, p0.y + a.y + b.y) + delta, 
				fmin(p0.z, p0.z + a.z + b.z) - delta, fmax(p0.z, p0.z + a.z + b.z) + delta));
}
																			

//===================================================================================
bool tzRectangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const 
{
	
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


//===================================================================================
void tzRectangle::setSampler(tzISampler* sampler) 
{
	sampler_ptr = sampler;
}


//===================================================================================
tzPoint3D tzRectangle::sample(const tzShadeRec& sr) 
{
	tzPoint2D samplePoint = sampler_ptr->sampleUnitSquare( sr.mRay );
	return (p0 + samplePoint.x * a + samplePoint.y * b);
}


//===================================================================================					 
tzNormal tzRectangle::getNormal(const tzPoint3D& p) 
{
	return (normal);
}


//===================================================================================
float tzRectangle::pdf(tzShadeRec& sr) 
{
	return (inv_area);
} 





