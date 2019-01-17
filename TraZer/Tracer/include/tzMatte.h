#ifndef TZ_TRACER_MATTE
#define TZ_TRACER_MATTE

#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"

//----------------------------------------------------------------------------- class Matte

class tzMatte: public tzIMaterial {
	public:
			
		tzMatte(void);											

		tzMatte(const tzMatte& m);
		
		virtual tzIMaterial* clone(void) const;									

		tzMatte& operator= (const tzMatte& rhs);

		~tzMatte(void);
		
		void setKa(const float k);
		
		void setKd(const float k);
		
		void setCd(const tzColor c);
		
		void setCd(const float r, const float g, const float b);
		
		void setCd(const float c);
				
		virtual tzColor shade(tzShadeRec& sr);

		virtual tzColor areaLightShade( tzShadeRec &sr ) const;

		virtual tzColor pathShade(tzShadeRec& sr);
		
	private:
		
		tzLambertian*		mAmbientBRDF;
		tzLambertian*		mDiffuseBRDF;
};


// ---------------------------------------------------------------- setKa
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void tzMatte::setKa(const float ka) 
{
	mAmbientBRDF->setKd(ka);
}


// ---------------------------------------------------------------- setKd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void tzMatte::setKd (const float kd) 
{
	mDiffuseBRDF->setKd(kd);
}


// ---------------------------------------------------------------- setCd

inline void tzMatte::setCd(const tzColor c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}


// ---------------------------------------------------------------- setCd

inline void tzMatte::setCd(const float r, const float g, const float b) 
{
	mAmbientBRDF->setCd(r, g, b);
	mDiffuseBRDF->setCd(r, g, b);
}

// ---------------------------------------------------------------- setCd

inline void tzMatte::setCd(const float c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}

#endif
