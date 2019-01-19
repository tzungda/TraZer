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
		setKs(const float ks);
		
		void
		setExp(const float exp);
		
		void
		setCs(const tzColor& c);
		
		void													
		setCs(const float r, const float g, const float b);
		
		void													
		setCs(const float c);
				
		//
		virtual tzColor
		shade(tzShadeRec& sr);

		//
		virtual tzColor areaLightShade( tzShadeRec &sr) const;
		
	private:
		
		tzLambertian*		mAmbientBRDF;
		tzLambertian*		mDiffuseBRDF;
		tzGlossySpecular*	mSpecularBRDF;
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
tzPhong::setKs(const float ks)
{
	mSpecularBRDF->setKs( ks );
}

inline void
tzPhong::setExp(const float exp)
{
	mSpecularBRDF->setExp( exp );
}

inline void
tzPhong::setCs(const tzColor& c)
{
	mSpecularBRDF->setCs( c );
}

inline void
tzPhong::setCs(const float r, const float g, const float b)
{
	mSpecularBRDF->setCs( r, g, b );
}

inline void
tzPhong::setCs(const float c)
{
	mSpecularBRDF->setCs( c );
}

#endif
