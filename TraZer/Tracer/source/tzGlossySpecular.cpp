
#include "../include/tzGlossySpecular.h"
#include "../include/tzMultiJittered.h"

// ---------------------------------------------------------------------- default constructor

tzGlossySpecular::tzGlossySpecular(void)
	: 	ks(0.5), 
		cs(1.0),
		sampler(NULL)
{}


// ---------------------------------------------------------------------- destructor

tzGlossySpecular::~tzGlossySpecular(void) {}


// ---------------------------------------------------------------------- clone

tzGlossySpecular*
tzGlossySpecular::clone (void) const {
	return (new tzGlossySpecular(*this));
}


// ---------------------------------------------------------------------- set_sampler
// this allows any type of sampling to be specified in the build functions

void
tzGlossySpecular::set_sampler(tzISampler* sp, const float exp) {
	sampler_ptr = sp;
	sampler_ptr->map_samples_to_hemisphere(exp);
}


// ---------------------------------------------------------------------- set_samples
// this sets up multi-jittered sampling using the number of samples

void
tzGlossySpecular::set_samples(const int num_samples, const float exp) {
	sampler_ptr = new tzMultiJittered(num_samples);
	sampler_ptr->map_samples_to_hemisphere(exp);
}
		

// ----------------------------------------------------------------------------------- f
// no sampling here: just use the Phong formula
// this is used for direct illumination only

RGBColor
tzGlossySpecular::f(const tzShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	RGBColor 	L;  				
	float 		ndotwi = (float)(sr.mNormal * wi);
	Vector3D 	r(-wi + 2.0 * sr.mNormal * ndotwi); // mirror reflection direction
	float 		rdotwo = (float)(r * wo);
		
	if (rdotwo > 0.0)
		L = ks * cs * pow(rdotwo, exp); 
			
	return (L);
}


// ----------------------------------------------------------------------------------- sample_f
// this is used for indirect illumination

RGBColor
tzGlossySpecular::sample_f(const tzShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
	
	float ndotwo = (float)(sr.mNormal * wo);
	Vector3D r = -wo + 2.0 * sr.mNormal * ndotwo;     // direction of mirror reflection
	
	Vector3D w = r;								
	Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w; 
	u.normalize();
	Vector3D v = u ^ w;
		
	Point3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;			// reflected ray direction
	
	if (sr.mNormal * wi < 0.0) 						// reflected ray is below tangent plane
		wi = -sp.x * u - sp.y * v + sp.z * w;

	float phong_lobe = (float)pow(r * wi, exp);
	pdf = phong_lobe * (float)(sr.mNormal * wi);

	return (ks * cs * phong_lobe);
}


// ----------------------------------------------------------------------------------- rho

RGBColor
tzGlossySpecular::rho(const tzShadeRec& sr, const Vector3D& wo) const {
	return (black);
}

