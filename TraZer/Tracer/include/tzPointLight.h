#ifndef TZ_TRACER_POINTLIGHT
#define TZ_TRACER_POINTLIGHT

#include "../include/tzShadeRec.h"
#include "../include/tzILight.h"


class tzPointLight: public tzILight {
	public:
	
		tzPointLight(void);

		tzPointLight(const tzPointLight& a);
	
		virtual std::shared_ptr<tzILight> clone(void) const;	
		
		tzPointLight& operator= (const tzPointLight& rhs);
		
		virtual ~tzPointLight(void);
		
		void setColor(const float c);
		
		void setColor(const tzColor& c);
		
		void setColor(const float r, const float g, const float b); 
		
		virtual tzVector3D getDirection( tzShadeRec& s) ;
		
		virtual tzColor L( tzShadeRec& s) ;

		// 
		void set_location( const tzVector3D &loc );
		void scaleRadiance(const float b);

		//
		virtual bool inShadow(const tzRay &ray, const tzShadeRec &sr) const;
	
	private:
	
		float		mLs;
		tzColor	mColor;
		tzVector3D	mLocation;
};



//===================================================================================
inline void tzPointLight::scaleRadiance(const float b) 
{
	mLs = b;
}

//===================================================================================
inline void tzPointLight::setColor(const float c) 
{
	mColor.r = c; mColor.g = c; mColor.b = c;
}

//===================================================================================
inline void tzPointLight::setColor(const tzColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzPointLight::setColor(const float r, const float g, const float b) 
{
	mColor.r = r; mColor.g = g; mColor.b = b;
}


#endif

