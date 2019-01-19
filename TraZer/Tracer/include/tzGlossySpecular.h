#ifndef TZ_TRACER_GLOSSY_SPECULAR
#define TZ_TRACER_GLOSSY_SPECULAR

#include "../include/tzIBRDF.h"

class tzGlossySpecular: public tzIBRDF {
	public:
	
		tzGlossySpecular(void);
		
		~tzGlossySpecular(void);
		
		virtual tzGlossySpecular*
		clone(void) const;
		
		virtual tzColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
				
		virtual tzColor
		sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
		
		void
		setKs(const float ks);
		
		void
		setExp(const float exp);
		
		void
		setCs(const tzColor& c);
		
		void													
		setCs(const float r, const float g, const float b);
		
		void													
		setCs(const float c);
		
		void setSampler(tzISampler* sp, const float exp);   			// any type of sampling
		
		void setSamples(const int numSamples, const float exp); 	// multi jittered sampling
		
		void setNormal(const tzNormal& n);
		
		
	private:
	
		float		mKs;
		tzColor 	mCs;			// specular color
		float		mExp; 		// specular exponent
		tzISampler*	mSampler;    // for use in sampleF
};


// -------------------------------------------------------------- setKs

inline void tzGlossySpecular::setKs(const float k) 
{
	mKs = k;
}

// -------------------------------------------------------------- setExp

inline void tzGlossySpecular::setExp(const float e)
{
	mExp= e;
}


// -------------------------------------------------------------- setCs

inline void tzGlossySpecular::setCs(const tzColor& c) 
{
	mCs = c;
}


// ---------------------------------------------------------------- setCs

inline void tzGlossySpecular::setCs(const float r, const float g, const float b) 
{
	mCs.r = r; mCs.g = g; mCs.b = b;
}


// ---------------------------------------------------------------- setCs

inline void tzGlossySpecular::setCs(const float c) 
{
	mCs.r = c; mCs.g = c; mCs.b = c;
}

#endif

