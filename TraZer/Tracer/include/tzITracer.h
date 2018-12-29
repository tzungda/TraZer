#ifndef TZ_TRACER_INTERFACE
#define TZ_TRACER_INTERFACE


#include "../include/tzConstants.h"
#include "../include/tzRay.h"
#include "RGBColor.h"

class tzWorld;

class tzITracer {
	public:
	
		tzITracer(void);									
		
		tzITracer( tzWorld* _world_ptr);						
				
		virtual											
		~tzITracer(void);									

		virtual RGBColor			// only overridden in the tzITracers SingleSphere and MultipleObjects
		trace_ray(const tzRay& ray) const;

		virtual RGBColor	
		trace_ray(const tzRay ray, const int depth) const;
				
	protected:
	
		tzWorld* world_ptr;
};

#endif
