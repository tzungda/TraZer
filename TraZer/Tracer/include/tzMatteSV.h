#ifndef TZ_TRACER_MATTE_SV
#define TZ_TRACER_MATTE_SV

#include "../include/tzIMaterial.h"
#include "../include/tzLambertianSV.h"
#include "../include/tzITexture.h"

//----------------------------------------------------------------------------- class Matte

class tzMatteSV: public tzIMaterial {
	public:
			
		tzMatteSV(void);

		tzMatteSV(const tzMatteSV& m);
		
		virtual tzIMaterial*										
		clone(void) const;									

		tzMatteSV&
		operator= (const tzMatteSV& rhs);

		~tzMatteSV(void);
		
		void 													
		set_ka(const float k);
		
		void 													
		set_kd(const float k);
		
		void													
		set_cd(const tzITexture *c);
				
		virtual tzRGBColor
		shade(tzShadeRec& sr);

		virtual tzRGBColor areaLightShade( tzShadeRec &sr ) const;

		virtual tzRGBColor pathShade(tzShadeRec& sr);
		
	private:
		
		tzLambertianSV*		ambient_brdf;
		tzLambertianSV*		diffuse_brdf;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void								
tzMatteSV::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void								
tzMatteSV::set_kd (const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void												
tzMatteSV::set_cd(const tzITexture *c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


#endif
