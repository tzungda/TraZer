
#ifndef TZ_TRACER_AREA_LIGHT
#define TZ_TRACER_AREA_LIGHT

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "RGBColor.h"
#include "Vector3D.h"
#include "Point3D.h"
#include "Normal.h"
#include "../include/tzILight.h"
#include "../include/tzIMaterial.h"
#include "../include/tzIGeometricObject.h"


class tzAreaLight: public tzILight {
	public:
	
		tzAreaLight(void);

		tzAreaLight(const tzAreaLight& al);

		virtual tzILight* clone(void) const;
		
		virtual 								
		~tzAreaLight(void);
		
		tzAreaLight&
		operator= (const tzAreaLight& rhs);
		
		void 									
		set_object(tzIGeometricObject* obj_ptr);
		
		virtual Vector3D												
		get_direction(tzShadeRec& s);
		
		virtual RGBColor						
		L(tzShadeRec& sr);
		
		virtual bool									
		in_shadow(const tzRay& ray, const tzShadeRec& sr) const;
		
		virtual float
		G(const tzShadeRec& sr) const;
		
		virtual float									
		pdf(const tzShadeRec& sr) const;
		
	private:
		
		tzIGeometricObject* 	object_ptr;
		tzIMaterial* 			material_ptr;	 // will be an emissive material
		Point3D				sample_point;
		Normal				light_normal;    // assigned in get_direction - which therefore can't be const for any light
		Vector3D			wi;			     // unit direction from hit point being shaded to sample point on light surface			
	
};


// --------------------------------------------------------------- set_object

inline void 									
tzAreaLight::set_object(tzIGeometricObject* obj_ptr) {
	object_ptr = obj_ptr;
	material_ptr = object_ptr->get_material();	
}

#endif
