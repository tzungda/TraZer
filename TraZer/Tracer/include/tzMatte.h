#ifndef TZ_TRACER_MATTE
#define TZ_TRACER_MATTE

#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"

//----------------------------------------------------------------------------- class Matte

class tzMatte: public tzIMaterial {
	public:
			
		tzMatte(void);											

		tzMatte(const tzMatte& m);
		
		virtual tzIMaterial*										
		clone(void) const;									

		tzMatte&
		operator= (const tzMatte& rhs);

		~tzMatte(void);
		
		void 													
		set_ka(const float k);
		
		void 													
		set_kd(const float k);
		
		void													
		set_cd(const tzRGBColor c);
		
		void													
		set_cd(const float r, const float g, const float b);
		
		void																						
		set_cd(const float c);
				
		virtual tzRGBColor
		shade(tzShadeRec& sr);

		virtual tzRGBColor areaLightShade( tzShadeRec &sr ) const;

		virtual tzRGBColor pathShade(tzShadeRec& sr);
		
	private:
		
		tzLambertian*		ambient_brdf;
		tzLambertian*		diffuse_brdf;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void								
tzMatte::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void								
tzMatte::set_kd (const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void												
tzMatte::set_cd(const tzRGBColor c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd

inline void													
tzMatte::set_cd(const float r, const float g, const float b) {
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
}

// ---------------------------------------------------------------- set_cd

inline void													
tzMatte::set_cd(const float c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}

#endif
