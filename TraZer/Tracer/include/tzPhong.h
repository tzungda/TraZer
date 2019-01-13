#ifndef TZ_TRACER_PHONG
#define TZ_TRACER_PHONG

#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"
#include "../include/tzGlossySpecular.h"

//----------------------------------------------------------------------------- class Matte

class tzPhong: public tzIMaterial {
	public:
			
		tzPhong(void);

		tzPhong(const tzPhong& m);
		
		virtual tzIMaterial*										
		clone(void) const;									

		tzPhong&
		operator= (const tzPhong& rhs);

		~tzPhong(void);
		
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

		// specular
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
				
		//
		virtual tzRGBColor
		shade(tzShadeRec& sr);

		//
		virtual tzRGBColor areaLightShade( tzShadeRec &sr) const;
		
	private:
		
		tzLambertian*		ambient_brdf;
		tzLambertian*		diffuse_brdf;
		tzGlossySpecular*	specular_brdf;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void								
tzPhong::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void								
tzPhong::set_kd (const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void												
tzPhong::set_cd(const tzRGBColor c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd

inline void													
tzPhong::set_cd(const float r, const float g, const float b) {
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
}

// ---------------------------------------------------------------- set_cd

inline void													
tzPhong::set_cd(const float c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}

//
inline void
tzPhong::set_ks(const float ks)
{
	specular_brdf->set_ks( ks );
}

inline void
tzPhong::set_exp(const float exp)
{
	specular_brdf->set_exp( exp );
}

inline void
tzPhong::set_cs(const tzRGBColor& c)
{
	specular_brdf->set_cs( c );
}

inline void
tzPhong::set_cs(const float r, const float g, const float b)
{
	specular_brdf->set_cs( r, g, b );
}

inline void
tzPhong::set_cs(const float c)
{
	specular_brdf->set_cs( c );
}

#endif
