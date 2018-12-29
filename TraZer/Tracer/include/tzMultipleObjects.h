#ifndef TZ_TRACER_MULTIPLE_OBJECTS
#define TZ_TRACER_MULTIPLE_OBJECTS



#include "../include/tzITracer.h"

class tzMultipleObjects: public tzITracer {
	public:
		
		tzMultipleObjects(void);
		
		tzMultipleObjects( tzWorld* _world_ptr);
	
		virtual											
		~tzMultipleObjects(void);
						
		virtual RGBColor trace_ray(const tzRay& ray) const;
		virtual RGBColor trace_ray(const tzRay ray, const int depth) const;
};

#endif

