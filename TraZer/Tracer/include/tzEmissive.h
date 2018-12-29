#ifndef TZ_TRACER_EMISSIVE
#define TZ_TRACER_EMISSIVE
 
#include "../include/tzIMaterial.h"
#include "../include/tzLambertian.h"

//----------------------------------------------------------------------------- class Matte

class tzEmissive: public tzIMaterial {
	private:
		float			ls;	//radiance scaling factor
		RGBColor		ce;	//color
	public:
			
		tzEmissive(void);

		tzEmissive(const tzEmissive& m);
		
		virtual tzIMaterial*										
		clone(void) const;									

		tzEmissive&
		operator= (const tzEmissive& rhs);

		~tzEmissive(void);
		
		float		emissiveRadiance( ) const;
		RGBColor	emissiveColor() const;

		void 													
		scale_radiance(const float k);
		
		void													
		set_ce(const RGBColor &c);
		
		void													
		set_ce(const float r, const float g, const float b);
		
		void																						
		set_ce(const float c);
				
		virtual RGBColor shade(tzShadeRec& sr);

		virtual RGBColor area_light_shade(tzShadeRec& sr);

		virtual RGBColor get_Le(tzShadeRec &sr);
		
};


#endif
