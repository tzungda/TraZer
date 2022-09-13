#ifndef TZ_TRACER_AMBIENTOCCLUDER
#define TZ_TRACER_AMBIENTOCCLUDER

#include "../include/tzISampler.h"
#include "../include/tzILight.h"

class tzAmbientOccluder: public tzILight {
	public:
	
		tzAmbientOccluder(void);

		tzAmbientOccluder(const tzAmbientOccluder& a);
	
		virtual std::shared_ptr<tzILight> clone(void) const;	
		
		tzAmbientOccluder&
		operator= (const tzAmbientOccluder& rhs);
		
		virtual ~tzAmbientOccluder(void);
				
		void
		scaleRadiance(const float b);
		
		void
		setColor(const float c);
		
		void
		setColor(const tzColor& c);
		
		void
		setColor(const float r, const float g, const float b); 
		
		virtual tzVector3D getDirection( tzShadeRec& s);
		
		virtual tzColor L( tzShadeRec& s);

		//
		virtual bool	inShadow(const tzRay &ray, const tzShadeRec &sr) const;

		//
		void		setSampler(std::shared_ptr< tzISampler > samplerPtr );

		//
		void		setMinAmount( float minAmount );
	
	private:
	
		float		mLs;
		tzColor	mColor;
		std::shared_ptr< tzISampler > mSamplerPtr;
		tzVector3D	mU, mV, mW;
		tzColor	mMinAmount;
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
inline void tzAmbientOccluder::setColor(const tzColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzAmbientOccluder::setColor(const float r, const float g, const float b) 
{
	mColor.r = r; mColor.g = g; mColor.b = b;
}


#endif

