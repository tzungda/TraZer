#ifndef TZ_TRACER_LIGHT
#define TZ_TRACER_LIGHT

#include "tzVector3D.h"
#include "tzRGBColor.h"
#include "../include/tzRay.h"

class tzShadeRec;


//-------------------------------------------------------------------- class Light

class tzILight {	
	public:
	
		tzILight(void);
								
		tzILight(const tzILight& ls);

		tzILight&
		operator= (const tzILight& rhs);

		virtual tzILight* clone(void) const = 0;
		
		virtual ~tzILight(void);
						
		virtual tzVector3D get_direction(tzShadeRec& sr) = 0;				
																
		virtual tzRGBColor L(tzShadeRec& sr);								

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
