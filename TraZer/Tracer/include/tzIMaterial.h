#ifndef TZ_TRACER_MATERIAL
#define TZ_TRACER_MATERIAL

#include "../include/tzWorld.h"			// required for the shade function in all derived classes
#include "tzColor.h"
#include <memory>

class tzIMaterial {	
	public:
	
		tzIMaterial(void);
		
		tzIMaterial(const tzIMaterial& material);
		virtual ~tzIMaterial(void);
		
		virtual std::shared_ptr<tzIMaterial> clone(void) const = 0;	
				
		virtual tzColor shade(tzShadeRec& sr);	

		virtual tzColor areaLightShade( tzShadeRec& sr) const;

		virtual tzColor getLe( tzShadeRec &sr );

		// for path trace
		virtual tzColor pathShade(tzShadeRec& sr);
		
	protected:
	
		tzIMaterial& operator= (const tzIMaterial& rhs);
};

//typedef std::shared_ptr<tzIMaterial> tzIMaterialPtr;

#endif

