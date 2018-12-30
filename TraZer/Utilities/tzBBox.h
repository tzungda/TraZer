#ifndef TZ_TRACER_BBOX
#define TZ_TRACER_BBOX


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


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

