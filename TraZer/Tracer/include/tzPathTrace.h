
#ifndef TZ_PATH_TRACE
#define TZ_PATH_TRACE

#include "../include/tzITracer.h"

class tzPathTrace: public tzITracer {
	public:
		
		tzPathTrace(void);
		
		tzPathTrace(tzWorld* _worldPtr);
	
		virtual tzRGBColor	
		trace_ray(const tzRay ray, const int depth) const;
		
		virtual tzRGBColor	
		trace_ray(const tzRay ray, double& tmin, const int depth) const;
};

#endif