#ifndef TZ_TRACER_MATTE
#define TZ_TRACER_MATTE

#include <memory>

#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"


class tzMatte: public tzIMaterial {
	public:
			
		tzMatte(void);											

		tzMatte(const tzMatte& m);
		
		virtual std::shared_ptr<tzIMaterial> clone(void) const;									

		tzMatte& operator= (const tzMatte& rhs);

		virtual ~tzMatte(void);
		
		void setKa(const float k);
		
		void setKd(const float k);
		
		void setCd(const tzColor c);
		
		void setCd(const float r, const float g, const float b);
		
		void setCd(const float c);
				
		virtual tzColor shade(tzShadeRec& sr);

		virtual tzColor areaLightShade( tzShadeRec &sr ) const;

		virtual tzColor pathShade(tzShadeRec& sr);
		
	private:
		
		std::shared_ptr< tzLambertian >	mAmbientBRDF;
		std::shared_ptr< tzLambertian >	mDiffuseBRDF;
};

typedef std::shared_ptr<tzMatte> tzMattePtr;

//===================================================================================
inline void tzMatte::setKa(const float ka) 
{
	mAmbientBRDF->setKd(ka);
}

//===================================================================================
inline void tzMatte::setKd (const float kd) 
{
	mDiffuseBRDF->setKd(kd);
}

//===================================================================================
inline void tzMatte::setCd(const tzColor c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}

//===================================================================================
inline void tzMatte::setCd(const float r, const float g, const float b) 
{
	mAmbientBRDF->setCd(r, g, b);
	mDiffuseBRDF->setCd(r, g, b);
}

//===================================================================================
inline void tzMatte::setCd(const float c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}

#endif
