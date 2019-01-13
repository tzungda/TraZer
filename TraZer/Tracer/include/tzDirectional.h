#ifndef TZ_TRACER_DIRECTIONAL
#define TZ_TRACER_DIRECTIONAL

#include "../include/tzILight.h"
#include "tzVector3D.h"
#include "tzRGBColor.h"

#include "../include/tzWorld.h"			// you will need this later on for shadows
#include "tzShadeRec.h"


class tzDirectional: public tzILight 
{
	public:
	
		tzDirectional(void);

		tzDirectional(const tzDirectional& dl);
		
		virtual tzILight* clone(void) const;			

		tzDirectional& operator= (const tzDirectional& rhs);
			
		virtual ~tzDirectional(void);
				
		void scaleRadiance(const float b);
		
		void setColor(const float c);
		
		void setColor(const tzRGBColor& c);
		
		void setColor(const float r, const float g, const float b); 		
			
		void setDirection(tzVector3D d);
		
		void setDirection(float dx, float dy, float dz);
		
		virtual tzVector3D getDirection( tzShadeRec& sr) ;
				
		virtual tzRGBColor L( tzShadeRec& sr) ;	

		bool inShadow(const tzRay &ray, const tzShadeRec &sr) const;
		
	private:

		float		mLs;			
		tzRGBColor	mColor;
		tzVector3D	mDir;		// direction the light comes from
};


// inlined access functions


//===================================================================================
inline void tzDirectional::scaleRadiance(const float b) 
{
	mLs = b;
}

//===================================================================================
inline void tzDirectional::setColor(const float c) 
{
	mColor.r = c; mColor.g = c; mColor.b = c;
}

//===================================================================================
inline void tzDirectional::setColor(const tzRGBColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzDirectional::setColor(const float r, const float g, const float b) 
{
	mColor.r = r; mColor.g = g; mColor.b = b;
}

//===================================================================================
inline void tzDirectional::setDirection(tzVector3D d) 
{
	mDir = d;
	mDir.normalize();
}

//===================================================================================
inline void tzDirectional::setDirection(float dx, float dy, float dz) 
{
	mDir.x = dx; mDir.y = dy; mDir.z = dz;
	mDir.normalize();
}


#endif

