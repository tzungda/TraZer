#ifndef TZ_TRACER_EMISSIVE
#define TZ_TRACER_EMISSIVE
 
#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"

//----------------------------------------------------------------------------- class Matte

class tzEmissive: public tzIMaterial {
	private:
		float			ls;	//radiance scaling factor
		tzRGBColor		ce;	//color
	public:
			
		tzEmissive(void);

		tzEmissive(const tzEmissive& m);
		
		virtual tzIMaterial*										
		clone(void) const;									

		tzEmissive&
		operator= (const tzEmissive& rhs);

		~tzEmissive(void);
		
		float		emissiveRadiance( ) const;
		tzRGBColor	emissiveColor() const;

		void 													
		scale_radiance(const float k);
		
		void													
		set_ce(const tzRGBColor &c);
		
		void													
		set_ce(const float r, const float g, const float b);
		
		void																						
		set_ce(const float c);
				
		virtual tzRGBColor shade(tzShadeRec& sr);

		virtual tzRGBColor area_light_shade( tzShadeRec& sr) const;

		virtual tzRGBColor get_Le(tzShadeRec &sr);
		
};


#endif
