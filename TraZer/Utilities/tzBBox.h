#ifndef TZ_TRACER_BBOX
#define TZ_TRACER_BBOX


#include "../Include/tzRay.h"
#include "tzPoint3D.h"

class tzBBox {
	public:
		float x0, x1, y0, y1, z0, z1;
		
		tzBBox(void);
						
		tzBBox(	const float x0, const float x1,
				const float y0, const float y1, 
				const float z0, const float z1);

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

