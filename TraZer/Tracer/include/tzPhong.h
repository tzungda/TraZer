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
		
		virtual std::shared_ptr<tzIMaterial> clone(void) const;									

		tzPhong& operator= (const tzPhong& rhs);

		virtual ~tzPhong(void);
		
		void setKa(const float k);
		
		void setKd(const float k);
		
		void setCd(const tzColor c);
		
		void setCd(const float r, const float g, const float b);
		
		void setCd(const float c);

		// specular
		void setKs(const float ks);
		
		void setExp(const float exp);
		
		void setCs(const tzColor& c);
		
		void setCs(const float r, const float g, const float b);
		
		void setCs(const float c);
				
		//
		virtual tzColor shade(tzShadeRec& sr);

		//
		virtual tzColor areaLightShade( tzShadeRec &sr) const;
		
	private:
		
		std::shared_ptr<tzLambertian>		mAmbientBRDF;
		std::shared_ptr<tzLambertian>		mDiffuseBRDF;
		std::shared_ptr<tzGlossySpecular>	mSpecularBRDF;
};

typedef std::shared_ptr< tzPhong > tzPhongPtr;

//===================================================================================
inline void tzPhong::setKa(const float ka) 
{
	mAmbientBRDF->setKd(ka);
}

//===================================================================================
inline void tzPhong::setKd (const float kd) 
{
	mDiffuseBRDF->setKd(kd);
}

//===================================================================================
inline void tzPhong::setCd(const tzColor c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}

//===================================================================================
inline void tzPhong::setCd(const float r, const float g, const float b)
{
	mAmbientBRDF->setCd(r, g, b);
	mDiffuseBRDF->setCd(r, g, b);
}

//===================================================================================
inline void tzPhong::setCd(const float c) 
{
	mAmbientBRDF->setCd(c);
	mDiffuseBRDF->setCd(c);
}

//===================================================================================
inline void tzPhong::setKs(const float ks)
{
	mSpecularBRDF->setKs( ks );
}

//===================================================================================
inline void tzPhong::setExp(const float exp)
{
	mSpecularBRDF->setExp( exp );
}

//===================================================================================
inline void tzPhong::setCs(const tzColor& c)
{
	mSpecularBRDF->setCs( c );
}

//===================================================================================
inline void tzPhong::setCs(const float r, const float g, const float b)
{
	mSpecularBRDF->setCs( r, g, b );
}

//===================================================================================
inline void tzPhong::setCs(const float c)
{
	mSpecularBRDF->setCs( c );
}

#endif
