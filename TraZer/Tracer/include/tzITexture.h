#ifndef TZ_INTERFACE_TEXTURE
#define TZ_INTERFACE_TEXTURE


#include <math.h>

#include "../include/tzShadeRec.h"
#include "tzRGBColor.h"

class tzITexture {	
	public:
	
		tzITexture(void);
		
		tzITexture(const tzITexture& texture);
				
		virtual tzITexture*
		clone(void) const = 0;	

		virtual 											
		~tzITexture(void);
		
		virtual tzRGBColor																			
		get_color(const tzShadeRec& sr) const = 0;
				
	protected:
	
		tzITexture&
		operator= (const tzITexture& rhs);
};

#endif
		