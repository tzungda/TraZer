#ifndef TZ_TRACER_SPHERICAL_MAP
#define TZ_TRACER_SPHERICAL_MAP


#include "../include/tzConstants.h"
#include "../include/tzIMapping.h"

class tzSphericalMap: public tzIMapping {
	public:
	
		tzSphericalMap(void);
		
		tzSphericalMap(const tzSphericalMap& sm);

		tzSphericalMap&
		operator= (const tzSphericalMap& rhs);

		virtual tzSphericalMap*
		clone(void) const;				

		~tzSphericalMap(void);
		
		virtual void
		get_texel_coordinates(	const 	tzPoint3D& 	local_hit_point, 
								const 	int 		xres, 
								const 	int 		yres, 
										int& 		row, 
										int& 		column) const;
};

#endif
		