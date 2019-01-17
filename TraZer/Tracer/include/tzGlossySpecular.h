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
		set_ks(const float ks);
		
		void
		set_exp(const float exp);
		
		void
		set_cs(const tzColor& c);
		
		void													
		set_cs(const float r, const float g, const float b);
		
		void													
		set_cs(const float c);
		
		void setSampler(tzISampler* sp, const float exp);   			// any type of sampling
		
		void setSamples(const int numSamples, const float exp); 	// multi jittered sampling
		
		void setNormal(const tzNormal& n);
		
		
	private:
	
		float		ks;
		tzColor 	cs;			// specular color
		float		exp; 		// specular exponent
		tzISampler*	sampler;    // for use in sampleF
};


// -------------------------------------------------------------- set_ks

inline void
tzGlossySpecular::set_ks(const float k) {
	ks = k;
}

// -------------------------------------------------------------- set_exp

inline void
tzGlossySpecular::set_exp(const float e) {
	exp= e;
}


// -------------------------------------------------------------- set_cs

inline void
tzGlossySpecular::set_cs(const tzColor& c) {
	cs = c;
}


// ---------------------------------------------------------------- set_cs

inline void													
tzGlossySpecular::set_cs(const float r, const float g, const float b) {
	cs.r = r; cs.g = g; cs.b = b;
}


// ---------------------------------------------------------------- set_cs

inline void													
tzGlossySpecular::set_cs(const float c) {
	cs.r = c; cs.g = c; cs.b = c;
}

#endif

