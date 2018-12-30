#ifndef TZ_TRACER_GLOSSY_SPECULAR
#define TZ_TRACER_GLOSSY_SPECULAR

#include "../include/tzIBRDF.h"

class tzGlossySpecular: public tzIBRDF {
	public:
	
		tzGlossySpecular(void);
		
		~tzGlossySpecular(void);
		
		virtual tzGlossySpecular*
		clone(void) const;
		
		virtual tzRGBColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
				
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi, float& pdf) const;
		
		virtual tzRGBColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
		
		void
		set_ks(const float ks);
		
		void
		set_exp(const float exp);
		
		void
		set_cs(const tzRGBColor& c);
		
		void													
		set_cs(const float r, const float g, const float b);
		
		void													
		set_cs(const float c);
		
		void
		set_sampler(tzISampler* sp, const float exp);   			// any type of sampling
		
		void
		set_samples(const int num_samples, const float exp); 	// multi jittered sampling
		
		void
		set_normal(const tzNormal& n);
		
		
	private:
	
		float		ks;
		tzRGBColor 	cs;			// specular color
		float		exp; 		// specular exponent
		tzISampler*	sampler;    // for use in sample_f
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
tzGlossySpecular::set_cs(const tzRGBColor& c) {
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

