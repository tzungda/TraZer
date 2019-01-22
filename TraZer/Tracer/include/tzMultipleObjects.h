#ifndef TZ_TRACER_MULTIPLE_OBJECTS
#define TZ_TRACER_MULTIPLE_OBJECTS



#include "../include/tzITracer.h"

class tzMultipleObjects: public tzITracer {
	public:
		
		tzMultipleObjects(void);
		
		tzMultipleObjects( tzWorld* _mWorldPtr);
	
		virtual ~tzMultipleObjects(void);
						
		virtual tzColor traceRay(const tzRay& ray) const;
		virtual tzColor traceRay(const tzRay ray, const int depth) const;
};

#endif

