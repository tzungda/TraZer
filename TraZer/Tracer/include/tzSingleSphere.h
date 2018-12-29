#ifndef TZ_SINGLESPHERE_TRACER
#define TZ_SINGLESPHERE_TRACER

#include "../include/tzITracer.h"

class tzSingleSphere: public tzITracer {
	public:
		
		tzSingleSphere(void);
										
		tzSingleSphere(tzWorld* _worldPtr);					
		
		virtual											
		~tzSingleSphere(void);
		
		virtual RGBColor	
		trace_ray(const tzRay& ray) const;
};

#endif

