#ifndef TZ_TRACER_EMISSIVE
#define TZ_TRACER_EMISSIVE
 
#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"

//----------------------------------------------------------------------------- class Matte

class tzEmissive: public tzIMaterial {
	private:
		float			ls;	//radiance scaling factor
		tzColor		ce;	//color
	public:
			
		tzEmissive(void);

		tzEmissive(const tzEmissive& m);
		
		virtual tzIMaterial*										
		clone(void) const;									

		tzEmissive&
		operator= (const tzEmissive& rhs);

		~tzEmissive(void);
		
		float		emissiveRadiance( ) const;
		tzColor	emissiveColor() const;

		void 													
		scaleRadiance(const float k);
		
		void													
		set_ce(const tzColor &c);
		
		void													
		set_ce(const float r, const float g, const float b);
		
		void																						
		set_ce(const float c);
				
		virtual tzColor shade(tzShadeRec& sr);

		virtual tzColor areaLightShade( tzShadeRec& sr) const;

		virtual tzColor getLe(tzShadeRec &sr);
		
};


#endif
