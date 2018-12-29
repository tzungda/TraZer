#ifndef TZ_TRACER_LIGHT
#define TZ_TRACER_LIGHT

#include "Vector3D.h"
#include "RGBColor.h"
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
						
		virtual Vector3D								
		get_direction(tzShadeRec& sr) = 0;				
																
		virtual RGBColor														
		L(tzShadeRec& sr);								

		virtual float G(const tzShadeRec& sr) const;

		//
		virtual bool			in_shadow( const tzRay &ray, const tzShadeRec &sr ) const;
		//
		virtual void			setCastsShadows( bool castsShadow );
		virtual bool			castsShadow( ) const;

		//
		virtual float			pdf( const tzShadeRec &sr ) const;

	protected:
		bool	mCastsShadows;
};

#endif
