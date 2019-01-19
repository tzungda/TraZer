
#ifndef TZ_TRACER_BTDF
#define TZ_TRACER_BTDF

#include <math.h>

#include "../include/tzConstants.h"
#include "tzVector3D.h"
#include "../include/tzShadeRec.h"

class tzIBTDF {
	public:
	
		tzIBTDF(void);
		
		tzIBTDF(const tzIBTDF& tzIBTDF);
		
		virtual tzIBTDF* clone(void) = 0;
		
		tzIBTDF& operator= (const tzIBTDF& rhs);
		
		virtual ~tzIBTDF(void);
				
		virtual tzColor f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const;
		
		virtual tzColor sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wt) const;
		
		virtual tzColor rho(const tzShadeRec& sr, const tzVector3D& wo) const;
};

#endif
