#ifndef TZ_TRACER_INTERFACE
#define TZ_TRACER_INTERFACE


#include "../include/tzConstants.h"
#include "../include/tzRay.h"
#include "tzRGBColor.h"

class tzWorld;

class tzITracer {
	public:
	
		tzITracer(void);									
		
		tzITracer( tzWorld* _mWorldPtr);						
				
		virtual											
		~tzITracer(void);									

		virtual tzRGBColor			// only overridden in the tzITracers SingleSphere and MultipleObjects
		traceRay(const tzRay& ray) const;

		virtual tzRGBColor
		traceRay(const tzRay ray, const int depth) const;
				
	protected:
	
		tzWorld* mWorldPtr;
};

#endif

