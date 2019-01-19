#ifndef TZ_INTERFACE_MAPPING
#define TZ_INTERFACE_MAPPING


#include <math.h>

#include "tzPoint3D.h"

class tzIMapping {		
	public:
		
		tzIMapping(void);
		
		tzIMapping(const tzIMapping& m);

		tzIMapping&
		operator= (const tzIMapping& rhs);

		virtual tzIMapping*
		clone(void) const = 0;				

		virtual ~tzIMapping(void);
				
		virtual void
		getTexelCoordinates(	const 	tzPoint3D& 	hit_point, 
								const 	int 		xres, 
								const 	int 		yres, 
										int& 		row, 
										int& 		column) const = 0;
};

#endif
		