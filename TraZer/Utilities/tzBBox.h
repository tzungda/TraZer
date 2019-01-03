#ifndef TZ_TRACER_BBOX
#define TZ_TRACER_BBOX


#include "../Include/tzRay.h"
#include "tzPoint3D.h"

class tzBBox {
	public:
		double x0, x1, y0, y1, z0, z1;
		
		tzBBox(void);
						
		tzBBox(	const double x0, const double x1,
				const double y0, const double y1, 
				const double z0, const double z1);

		tzBBox(const tzPoint3D p0, const tzPoint3D p1);

		tzBBox(const tzBBox& bbox);

		tzBBox&
		operator= (const tzBBox& rhs);

		~tzBBox(void);
		
		bool 															
		hit(const tzRay& ray) const;
	
		bool
		inside(const tzPoint3D& p) const;
};
	
#endif

