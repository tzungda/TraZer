#ifndef TZ_TRACER_MATERIAL
#define TZ_TRACER_MATERIAL

#include "../include/tzWorld.h"			// required for the shade function in all derived classes
#include "tzRGBColor.h"


class tzIMaterial {	
	public:
	
		tzIMaterial(void);
		
		tzIMaterial(const tzIMaterial& material);
		
		virtual tzIMaterial*
		clone(void) const = 0;	
				
		virtual ~tzIMaterial(void);
				
		virtual tzRGBColor
		shade(tzShadeRec& sr);	

		virtual tzRGBColor area_light_shade( tzShadeRec& sr) const;

		virtual tzRGBColor get_Le( tzShadeRec &sr );

		// for path trace
		virtual tzRGBColor path_shade(tzShadeRec& sr);
		
	protected:
	
		tzIMaterial&
		operator= (const tzIMaterial& rhs);
};

#endif

