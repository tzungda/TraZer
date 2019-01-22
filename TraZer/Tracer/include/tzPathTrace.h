
#ifndef TZ_PATH_TRACE
#define TZ_PATH_TRACE

#include "../include/tzITracer.h"

class tzPathTrace: public tzITracer {
	public:
		
		tzPathTrace(void);
		
		tzPathTrace(tzWorld* _worldPtr);
	
		virtual tzColor traceRay(const tzRay ray, const int depth) const;
		
		virtual tzColor traceRay(const tzRay ray, float& tmin, const int depth) const;
};

#endif
