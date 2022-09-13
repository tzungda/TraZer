#ifndef TZ_TRACER_DIRECTIONAL
#define TZ_TRACER_DIRECTIONAL

#include "../include/tzILight.h"
#include "tzVector3D.h"
#include "tzColor.h"

#include "../include/tzWorld.h"			// you will need this later on for shadows
#include "tzShadeRec.h"


class tzDirectionalLight: public tzILight 
{
	public:
	
		tzDirectionalLight(void);

		tzDirectionalLight(const tzDirectionalLight& dl);
		
		virtual std::shared_ptr<tzILight> clone(void) const;			

		tzDirectionalLight& operator= (const tzDirectionalLight& rhs);
			
		virtual ~tzDirectionalLight(void);
				
		void scaleRadiance(const float b);
		
		void setColor(const float c);
		
		void setColor(const tzColor& c);
		
		void setColor(const float r, const float g, const float b); 		
			
		void setDirection(tzVector3D d);
		
		void setDirection(float dx, float dy, float dz);
		
		virtual tzVector3D getDirection( tzShadeRec& sr) ;
				
		virtual tzColor L( tzShadeRec& sr) ;	

		bool inShadow(const tzRay &ray, const tzShadeRec &sr) const;
		
	private:

		float mLs;
		tzColor	mColor;
		tzVector3D	mDir;		// direction the light comes from
};


// inlined access functions


//===================================================================================
inline void tzDirectionalLight::scaleRadiance(const float b) 
{
	mLs = b;
}

//===================================================================================
inline void tzDirectionalLight::setColor(const float c) 
{
	mColor.r = c; mColor.g = c; mColor.b = c;
}

//===================================================================================
inline void tzDirectionalLight::setColor(const tzColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzDirectionalLight::setColor(const float r, const float g, const float b) 
{
	mColor.r = r; mColor.g = g; mColor.b = b;
}

//===================================================================================
inline void tzDirectionalLight::setDirection(tzVector3D d) 
{
	mDir = d;
	mDir.normalize();
}

//===================================================================================
inline void tzDirectionalLight::setDirection(float dx, float dy, float dz) 
{
	mDir.x = dx; mDir.y = dy; mDir.z = dz;
	mDir.normalize();
}


#endif

