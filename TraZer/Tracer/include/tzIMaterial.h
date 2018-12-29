#ifndef TZ_TRACER_MATERIAL
#define TZ_TRACER_MATERIAL

#include "../include/tzWorld.h"			// required for the shade function in all derived classes
#include "RGBColor.h"
#include "../include/tzScene.h"

class tzIMaterial {	
	public:
	
		tzIMaterial(void);
		
		tzIMaterial(const tzIMaterial& material);
		
		virtual tzIMaterial*
		clone(void) const = 0;	
				
		virtual ~tzIMaterial(void);
				
		virtual RGBColor
		shade(tzShadeRec& sr);	

		virtual RGBColor area_light_shade(tzShadeRec& sr);

		virtual RGBColor get_Le( tzShadeRec &sr );
		
	protected:
	
		tzIMaterial&
		operator= (const tzIMaterial& rhs);
};

#endif

