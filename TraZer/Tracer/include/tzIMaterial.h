#ifndef TZ_TRACER_MATERIAL
#define TZ_TRACER_MATERIAL

#include "../include/tzWorld.h"			// required for the shade function in all derived classes
#include "tzColor.h"


class tzIMaterial {	
	public:
	
		tzIMaterial(void);
		
		tzIMaterial(const tzIMaterial& material);
		
		virtual tzIMaterial* clone(void) const = 0;	
				
		virtual ~tzIMaterial(void);
				
		virtual tzColor shade(tzShadeRec& sr);	

		virtual tzColor areaLightShade( tzShadeRec& sr) const;

		virtual tzColor getLe( tzShadeRec &sr );

		// for path trace
		virtual tzColor pathShade(tzShadeRec& sr);
		
	protected:
	
		tzIMaterial&
		operator= (const tzIMaterial& rhs);
};

#endif

