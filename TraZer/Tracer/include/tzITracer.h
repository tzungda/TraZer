#ifndef TZ_TRACER_INTERFACE
#define TZ_TRACER_INTERFACE


#include "../include/tzConstants.h"
#include "../include/tzRay.h"
#include "tzColor.h"

class tzWorld;

class tzITracer {
	public:
	
		tzITracer(void);									
		
		tzITracer( tzWorld* _mWorldPtr);						
				
		virtual											
		~tzITracer(void);									

		virtual tzColor			// only overridden in the tzITracers SingleSphere and MultipleObjects
		traceRay(const tzRay& ray) const;

		virtual tzColor
		traceRay(const tzRay ray, const int depth) const;
				
	protected:
	
		tzWorld* mWorldPtr;
};

#endif

