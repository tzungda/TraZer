#ifndef TZ_TRACER_REFLECTIVE
#define TZ_TRACER_REFLECTIVE


#include "../include/tzPhong.h"
#include "../include/tzPerfectSpecular.h"

class tzReflective: public tzPhong {	
	public:
	
		tzReflective(void);

		tzReflective(const tzReflective& rm);

		tzReflective&
		operator= (const tzReflective& rhs);

		virtual std::shared_ptr<tzIMaterial> clone(void) const;

		~tzReflective(void);
		
		void
		setKr(const float k);
				
		void													
		setCr(const tzColor& c);
		
		void
		setCr(const float r, const float g, const float b);
		
		void
		setCr(const float c);
		
		virtual tzColor										
		shade(tzShadeRec& s);

		virtual tzColor pathShade(tzShadeRec& sr);
		
	private:
	
		std::shared_ptr<tzPerfectSpecular> mReflectiveBRDF;
};


//===================================================================================
inline void tzReflective::setKr(const float k) 
{
	mReflectiveBRDF->setKr(k);
}

//===================================================================================
inline void tzReflective::setCr(const tzColor& c) 
{
	mReflectiveBRDF->setCr(c);
}

//===================================================================================
inline void tzReflective::setCr(const float r, const float g, const float b) 
{
	mReflectiveBRDF->setCr(r, g, b);
}

//===================================================================================
inline void tzReflective::setCr(const float c) 
{
	mReflectiveBRDF->setCr(c);
}

#endif
