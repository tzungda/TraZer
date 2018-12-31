
#include "../include/tzPerfectTransmitter.h"

// ------------------------------------------------------------------- default constructor

tzPerfectTransmitter::tzPerfectTransmitter(void)
	: 	tzIBTDF(),
		kt(0.0), 
		ior(1.0)
{}


// ------------------------------------------------------------------- copy constructor

tzPerfectTransmitter::tzPerfectTransmitter(const tzPerfectTransmitter& pt)
	: 	tzIBTDF(pt),
		kt(pt.kt), 
		ior(pt.ior)
{}


// ------------------------------------------------------------------- clone

tzPerfectTransmitter*
tzPerfectTransmitter::clone(void) {
	return (new tzPerfectTransmitter(*this));
}


// ------------------------------------------------------------------- destructor

tzPerfectTransmitter::~tzPerfectTransmitter(void) {}



// ------------------------------------------------------------------- assignment operator
		
tzPerfectTransmitter&
tzPerfectTransmitter::operator= (const tzPerfectTransmitter& rhs) {
	if (this == &rhs)
		return (*this);
		
	kt = rhs.kt;
	ior = rhs.ior;

	return (*this);
}


// ------------------------------------------------------------------- tir
// tests for total internal reflection

bool													
tzPerfectTransmitter::tir(const tzShadeRec& sr) const {
	tzVector3D wo(-sr.mRay.d); 
	float cos_thetai = sr.mNormal * wo;  
	float eta = ior;
	
	if (cos_thetai < 0.0) 
		eta = 1.0 / eta; 
		
	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}	


// ------------------------------------------------------------------- f

tzRGBColor
tzPerfectTransmitter::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------- sample_f
// this computes the direction wt for perfect transmission
// and returns the transmission coefficient
// this is only called when there is no total internal reflection

tzRGBColor
tzPerfectTransmitter::sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wt) const {
	
	tzNormal n(sr.mNormal);
	float cos_thetai = n * wo;
	float eta = ior;	
		
	if (cos_thetai < 0.0) {			// transmitted ray is outside     
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0 / eta; 			// invert ior 
	}

	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;   
	
	return (kt / (eta * eta) * white / fabs(sr.mNormal * wt));
}


// ------------------------------------------------------------------- rho

tzRGBColor
tzPerfectTransmitter::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (black);
}


