
#include "../include/tzGlossySpecular.h"
#include "../include/tzMultiJittered.h"

//===================================================================================
tzGlossySpecular::tzGlossySpecular(void)
	: 	mKs(0.5), 
		mCs(1.0),
		mSampler(NULL)
{}


//===================================================================================
tzGlossySpecular::~tzGlossySpecular(void) {}


//===================================================================================
tzGlossySpecular* tzGlossySpecular::clone (void) const 
{
	return (new tzGlossySpecular(*this));
}


//===================================================================================
void tzGlossySpecular::setSampler(tzISampler* sp, const float exp) 
{
	mSamplerPtr = sp;
	mSamplerPtr->mapSamplesToHemisphere(exp);
}


//===================================================================================
void tzGlossySpecular::setSamples(const int numSamples, const float exp) 
{
	mSamplerPtr = new tzMultiJittered(numSamples);
	mSamplerPtr->mapSamplesToHemisphere(exp);
}
		

//===================================================================================
tzColor tzGlossySpecular::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const 
{
	tzColor 	L;
	float 		ndotwi = (float)(sr.mNormal * wi);
	tzVector3D 	r(-wi + 2.0f * sr.mNormal * ndotwi); // mirror reflection direction
	float 		rdotwo = (float)(r * wo);
		
	if (rdotwo > 0.0f)
		L = mKs * mCs * pow(rdotwo, mExp); 
			
	return (L);
}


//===================================================================================
tzColor tzGlossySpecular::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const 
{
	
	float ndotwo = (float)(sr.mNormal * wo);
	tzVector3D r = -wo + 2.0 * sr.mNormal * ndotwo;     // direction of mirror reflection
	
	tzVector3D w = r;
	tzVector3D u = tzVector3D(0.00424f, 1.0f, 0.00764f) ^ w;
	u.normalize();
	tzVector3D v = u ^ w;
		
	tzPoint3D sp = mSamplerPtr->sampleHemisphere(sr.mRay);
	wi = sp.x * u + sp.y * v + sp.z * w;			// reflected ray direction
	
	if (sr.mNormal * wi < 0.0) 						// reflected ray is below tangent plane
		wi = -sp.x * u - sp.y * v + sp.z * w;

	float phong_lobe = (float)pow(r * wi, mExp);
	pdf = phong_lobe * (float)(sr.mNormal * wi);

	return (mKs * mCs * phong_lobe);
}


//===================================================================================
tzColor
tzGlossySpecular::rho(const tzShadeRec& sr, const tzVector3D& wo) const 
{
	return (black);
}

