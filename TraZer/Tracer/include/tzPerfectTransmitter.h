
#ifndef TZ_TRACER_PERFECT_TRANSMITTER
#define TZ_TRACER_PERFECT_TRANSMITTER

// this implements perfect specular transmission for transparent materials

#include "../include/tzIBTDF.h"

class tzPerfectTransmitter: public tzIBTDF {
	public:
	
		tzPerfectTransmitter(void);
		
		tzPerfectTransmitter(const tzPerfectTransmitter& pt);
		
		virtual std::shared_ptr<tzIBTDF> clone(void);
		
		~tzPerfectTransmitter(void);
		
		tzPerfectTransmitter& operator= (const tzPerfectTransmitter& rhs);
		
		void set_kt(const float k);
		
		void set_ior(const float eta);

		bool tir(const tzShadeRec& sr) const;
		
		virtual tzColor f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wt) const;
		
		virtual tzColor rho(const tzShadeRec& sr, const tzVector3D& wo) const;
				
	private:
	
		float	mKt;			// transmission coefficient
		float	mIor;		// index of refraction
};

//===================================================================================
inline void tzPerfectTransmitter::set_kt(const float k) 
{
	mKt = k;
}

//===================================================================================
inline void tzPerfectTransmitter::set_ior(const float eta) 
{
	mIor = eta;
}

#endif
