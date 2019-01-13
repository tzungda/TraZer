#ifndef TZ_TRACER_WHITTED
#define TZ_TRACER_WHITTED


#include "../include/tzITracer.h"

class tzWhitted: public tzITracer {
	public:
		
		tzWhitted(void);
		
		tzWhitted(tzWorld* _worldPtr);
		
		virtual	~tzWhitted(void);
	
		virtual tzRGBColor traceRay(const tzRay ray, const int depth) const;
};

#endif
