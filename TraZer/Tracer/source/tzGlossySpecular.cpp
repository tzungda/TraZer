
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

tzRGBColor
tzGlossySpecular::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	tzRGBColor 	L;
	float 		ndotwi = (float)(sr.mNormal * wi);
	tzVector3D 	r(-wi + 2.0 * sr.mNormal * ndotwi); // mirror reflection direction
	float 		rdotwo = (float)(r * wo);
		
	if (rdotwo > 0.0)
		L = ks * cs * pow(rdotwo, exp); 
			
	return (L);
}


// ----------------------------------------------------------------------------------- sample_f
// this is used for indirect illumination

tzRGBColor
tzGlossySpecular::sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const {
	
	float ndotwo = (float)(sr.mNormal * wo);
	tzVector3D r = -wo + 2.0 * sr.mNormal * ndotwo;     // direction of mirror reflection
	
	tzVector3D w = r;
	tzVector3D u = tzVector3D(0.00424f, 1.0f, 0.00764f) ^ w;
	u.normalize();
	tzVector3D v = u ^ w;
		
	tzPoint3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;			// reflected ray direction
	
	if (sr.mNormal * wi < 0.0) 						// reflected ray is below tangent plane
		wi = -sp.x * u - sp.y * v + sp.z * w;

	float phong_lobe = (float)pow(r * wi, exp);
	pdf = phong_lobe * (float)(sr.mNormal * wi);

	return (ks * cs * phong_lobe);
}


// ----------------------------------------------------------------------------------- rho

tzRGBColor
tzGlossySpecular::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (black);
}

