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
		
		virtual tzIMaterial* clone(void) const;									

		tzMatteSV& operator= (const tzMatteSV& rhs);

		~tzMatteSV(void);
		
		void setKa(const float k);
		
		void setKd(const float k);
		
		void setCd(const tzITexture *c);
				
		virtual tzColor shade(tzShadeRec& sr);

		virtual tzColor areaLightShade( tzShadeRec &sr ) const;

		virtual tzColor pathShade(tzShadeRec& sr);
		
	private:
		
		tzLambertianSV*		mAmbientBRDF;
		tzLambertianSV*		mDiffuseBRDF;
};


// ---------------------------------------------------------------- setKa
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void tzMatteSV::setKa(const float ka)
{
	mAmbientBRDF->setKd(ka);
}


// ---------------------------------------------------------------- setKd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void tzMatteSV::setKd (const float kd) 
{
	mDiffuseBRDF->setKd(kd);
}


// ---------------------------------------------------------------- setCd

inline void tzMatteSV::setCd(const tzITexture *c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}


#endif
