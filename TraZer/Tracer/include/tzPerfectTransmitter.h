
#ifndef TZ_TRACER_PERFECT_TRANSMITTER
#define TZ_TRACER_PERFECT_TRANSMITTER

// this implements perfect specular transmission for transparent materials

#include "../include/tzIBTDF.h"

class tzPerfectTransmitter: public tzIBTDF {
	public:
	
		tzPerfectTransmitter(void);
		
		tzPerfectTransmitter(const tzPerfectTransmitter& pt);
		
		virtual tzPerfectTransmitter*
		clone(void);
		
		~tzPerfectTransmitter(void);
		
		tzPerfectTransmitter&
		operator= (const tzPerfectTransmitter& rhs);
		
		void
		set_kt(const float k);
		
		void
		set_ior(const float eta);

		bool													
		tir(const tzShadeRec& sr) const;
		
		virtual tzRGBColor
		f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzRGBColor
		sample_f(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wt) const;
		
		virtual tzRGBColor
		rho(const tzShadeRec& sr, const tzVector3D& wo) const;
				
	private:
	
		float	kt;			// transmission coefficient
		float	ior;		// index of refraction
};


// -------------------------------------------------------------- set_kt

inline void
tzPerfectTransmitter::set_kt(const float k) {
	kt = k;
}

// -------------------------------------------------------------- set_ior

inline void
tzPerfectTransmitter::set_ior(const float eta) {
	ior = eta;
}

#endif
