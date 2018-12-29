
#ifndef TZ_TRACER_LAMBERTIAN
#define TZ_TRACER_LAMBERTIAN


// This implements a perfect diffuse scatterer

#include "../include/tzIBRDF.h"

class tzLambertian: public tzIBRDF
{
	public:
	
		tzLambertian(void);
		
		tzLambertian(const tzLambertian& lamb);
		
		tzLambertian&
		operator= (const tzLambertian& rhs);
		
		virtual ~tzLambertian(void);
		
		virtual tzLambertian*
		clone(void) const;
		
		virtual RGBColor
		f(const tzShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
		
		virtual RGBColor												
		sample_f(const tzShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
		
		virtual RGBColor
		rho(const tzShadeRec& sr, const Vector3D& wo) const;
			
		void
		set_ka(const float ka);	
				
		void
		set_kd(const float kd);
		
		void
		set_cd(const RGBColor& c);
		
		void													
		set_cd(const float r, const float g, const float b);
		
		void													
		set_cd(const float c);
					
	private:
	
		float		kd;
		RGBColor 	cd;
};




// -------------------------------------------------------------- set_ka

inline void
tzLambertian::set_ka(const float k) {
	kd = k;
}



// -------------------------------------------------------------- set_kd

inline void
tzLambertian::set_kd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- set_cd

inline void
tzLambertian::set_cd(const RGBColor& c) {
	cd = c;
}


// ---------------------------------------------------------------- set_cd

inline void													
tzLambertian::set_cd(const float r, const float g, const float b) {
	cd.r = r; cd.g = g; cd.b = b;
}


// ---------------------------------------------------------------- set_cd

inline void													
tzLambertian::set_cd(const float c) {
	cd.r = c; cd.g = c; cd.b = c;
}

#endif
