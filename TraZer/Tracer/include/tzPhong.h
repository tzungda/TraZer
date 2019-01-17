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
		setKa(const float k);
		
		void 													
		setKd(const float k);
		
		void													
		setCd(const tzColor c);
		
		void													
		setCd(const float r, const float g, const float b);
		
		void																						
		setCd(const float c);

		// specular
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
				
		//
		virtual tzColor
		shade(tzShadeRec& sr);

		//
		virtual tzColor areaLightShade( tzShadeRec &sr) const;
		
	private:
		
		tzLambertian*		mAmbientBRDF;
		tzLambertian*		mDiffuseBRDF;
		tzGlossySpecular*	specular_brdf;
};


// ---------------------------------------------------------------- setKa
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void								
tzPhong::setKa(const float ka) {
	mAmbientBRDF->setKd(ka);
}


// ---------------------------------------------------------------- setKd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void								
tzPhong::setKd (const float kd) {
	mDiffuseBRDF->setKd(kd);
}


// ---------------------------------------------------------------- setCd

inline void												
tzPhong::setCd(const tzColor c) {
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}


// ---------------------------------------------------------------- setCd

inline void													
tzPhong::setCd(const float r, const float g, const float b) {
	mAmbientBRDF->setCd(r, g, b);
	mDiffuseBRDF->setCd(r, g, b);
}

// ---------------------------------------------------------------- setCd

inline void													
tzPhong::setCd(const float c) {
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
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
tzPhong::set_cs(const tzColor& c)
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
