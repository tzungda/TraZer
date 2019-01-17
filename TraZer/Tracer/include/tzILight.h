#ifndef TZ_TRACER_LIGHT
#define TZ_TRACER_LIGHT

#include "tzVector3D.h"
#include "tzColor.h"
#include "../include/tzRay.h"

class tzShadeRec;

class tzILight 
{	
	public:
	
		tzILight(void);
								
		tzILight(const tzILight& ls);

		tzILight&
		operator= (const tzILight& rhs);

		virtual tzILight* clone(void) const = 0;
		
		virtual ~tzILight(void);
						
		virtual tzVector3D getDirection( tzShadeRec& sr) = 0;
																
		virtual tzColor L( tzShadeRec& sr) ;								

		virtual float G(const tzShadeRec& sr) const;

		//
		virtual bool			inShadow( const tzRay &ray, const tzShadeRec &sr ) const;
		//
		virtual void			setCastsShadows( bool castsShadow );
		virtual bool			castsShadow( ) const;

		//
		virtual float			pdf( const tzShadeRec &sr ) const;

	protected:
		bool	mCastsShadows;
};

#endif
