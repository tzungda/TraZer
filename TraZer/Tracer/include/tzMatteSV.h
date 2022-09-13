#ifndef TZ_TRACER_MATTE_SV
#define TZ_TRACER_MATTE_SV

#include "../include/tzIMaterial.h"
#include "../include/tzLambertianSV.h"
#include "../include/tzITexture.h"


class tzMatteSV: public tzIMaterial {
	public:
			
		tzMatteSV(void);

		tzMatteSV(const tzMatteSV& m);

		virtual ~tzMatteSV(void);

		tzMatteSV& operator= (const tzMatteSV& rhs);

		void setKa(const float k);

		void setKd(const float k);

		void setCd(const std::shared_ptr<tzITexture> c);
		
		virtual std::shared_ptr<tzIMaterial> clone(void) const;									
				
		virtual tzColor shade(tzShadeRec& sr);

		virtual tzColor areaLightShade( tzShadeRec &sr ) const;

		virtual tzColor pathShade(tzShadeRec& sr);
		
	private:
		
		std::shared_ptr<tzLambertianSV> mAmbientBRDF;
		std::shared_ptr<tzLambertianSV> mDiffuseBRDF;
};

typedef std::shared_ptr< tzMatteSV> tzMatteSVPtr;

//===================================================================================
inline void tzMatteSV::setKa(const float ka)
{
	mAmbientBRDF->setKd(ka);
}

//===================================================================================
inline void tzMatteSV::setKd (const float kd) 
{
	mDiffuseBRDF->setKd(kd);
}

//===================================================================================
inline void tzMatteSV::setCd(const std::shared_ptr<tzITexture> c)
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}


#endif
