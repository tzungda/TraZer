#ifndef TZ_TRACER_AMBIENT
#define TZ_TRACER_AMBIENT


#include "../include/tzILight.h"

class tzAmbient: public tzILight 
{
	public:
	
		tzAmbient(void);

		tzAmbient(const tzAmbient& a);
	
		virtual tzILight* clone(void) const;	
		
		tzAmbient& operator= (const tzAmbient& rhs);
		
		virtual ~tzAmbient(void);
				
		void scaleRadiance(const float b);
		
		void setColor(const float c);
		
		void setColor(const tzRGBColor& c);
		
		void setColor(const float r, const float g, const float b); 
		
		virtual tzVector3D getDirection(  tzShadeRec& s) ; 
		
		virtual tzRGBColor L(  tzShadeRec& s) ;
	
	private:
	
		float		mLs;
		tzRGBColor	mColor;
};


//===================================================================================
inline void tzAmbient::scaleRadiance(const float b) 
{
	mLs = b;
}

//===================================================================================
inline void tzAmbient::setColor(const float c) 
{
	mColor.r = c; mColor.g = c; mColor.b = c;
}

//===================================================================================
inline void tzAmbient::setColor(const tzRGBColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzAmbient::setColor(const float r, const float g, const float b) 
{
	mColor.r = r; mColor.g = g; mColor.b = b;
}


#endif

