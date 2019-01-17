#ifndef TZ_TRACER_AREA_LIGHTING
#define TZ_TRACER_AREA_LIGHTING


#include "../include/tzITracer.h"

class tzAreaLighting: public tzITracer {
	public:
		
		tzAreaLighting(void);
		
		tzAreaLighting(tzWorld* _worldPtr);
		
		virtual ~tzAreaLighting(void);
						
		virtual tzColor traceRay(const tzRay ray, const int depth) const;
		
};

#endif
