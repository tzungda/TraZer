#ifndef TZ_TRACER_AMBIENTOCCLUDER
#define TZ_TRACER_AMBIENTOCCLUDER

#include "../include/tzISampler.h"
#include "../include/tzILight.h"

class tzAmbientOccluder: public tzILight {
	public:
	
		tzAmbientOccluder(void);

		tzAmbientOccluder(const tzAmbientOccluder& a);
	
		virtual tzILight*
		clone(void) const;	
		
		tzAmbientOccluder&
		operator= (const tzAmbientOccluder& rhs);
		
		virtual ~tzAmbientOccluder(void);
				
		void
		scaleRadiance(const float b);
		
		void
		setColor(const float c);
		
		void
		setColor(const tzRGBColor& c);
		
		void
		setColor(const float r, const float g, const float b); 
		
		virtual tzVector3D getDirection( tzShadeRec& s);
		
		virtual tzRGBColor L( tzShadeRec& s);

		//
		virtual bool	inShadow(const tzRay &ray, const tzShadeRec &sr) const;

		//
		void		setSampler( tzISampler *samplerPtr );

		//
		void		setMinAmount( float minAmount );
	
	private:
	
		float		mLs;
		tzRGBColor	mColor;
		tzISampler	*mSamplerPtr;
		tzVector3D	u, v, w;
		tzRGBColor	mMinAmount;
};



//===================================================================================
inline void tzAmbientOccluder::scaleRadiance(const float b) 
{
	mLs = b;
}

//===================================================================================
inline void tzAmbientOccluder::setColor(const float c) 
{
	mColor.r = c; mColor.g = c; mColor.b = c;
}

//===================================================================================
inline void tzAmbientOccluder::setColor(const tzRGBColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzAmbientOccluder::setColor(const float r, const float g, const float b) 
{
	mColor.r = r; mColor.g = g; mColor.b = b;
}


#endif

