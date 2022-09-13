#ifndef TZ_TRACER_RAY_CAST
#define TZ_TRACER_RAY_CAST

#include "../include/tzITracer.h"

class tzRayCast: public tzITracer {
	public:
		
		tzRayCast(void) = delete;
		
		tzRayCast(tzWorld& _worldPtr);
				
		virtual ~tzRayCast(void);

		virtual tzColor traceRay(const tzRay ray, const int depth) const;
};

#endif
