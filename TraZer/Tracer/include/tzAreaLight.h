#ifndef TZ_TRACER_AREA_LIGHT
#define TZ_TRACER_AREA_LIGHT


#include "tzRGBColor.h"
#include "tzVector3D.h"
#include "tzPoint3D.h"
#include "tzNormal.h"
#include "../include/tzILight.h"
#include "../include/tzIMaterial.h"
#include "../include/tzIGeometricObject.h"


class tzAreaLight: public tzILight {
	public:
	
		tzAreaLight(void);

		tzAreaLight(const tzAreaLight& al);

		virtual tzILight* clone(void) const;
		
		virtual ~tzAreaLight(void);
		
		tzAreaLight& operator= (const tzAreaLight& rhs);
		
		void 									
		set_object(tzIGeometricObject* obj_ptr);
		
		virtual tzVector3D getDirection( tzShadeRec& s) ;
		
		virtual tzRGBColor L( tzShadeRec& sr) ;
		
		virtual bool inShadow(const tzRay& ray, const tzShadeRec& sr) const;
		
		virtual float G(const tzShadeRec& sr) const;
		
		virtual float pdf(const tzShadeRec& sr) const;
		
	private:
		
		tzIGeometricObject* 	object_ptr;
		tzIMaterial* 			material_ptr;	 // will be an emissive material
		tzPoint3D				sample_point;
		tzNormal				light_normal;    // assigned in get_direction - which therefore can't be const for any light
		tzVector3D				wi;			     // unit direction from hit point being shaded to sample point on light surface			
	
};


// --------------------------------------------------------------- set_object

inline void 									
tzAreaLight::set_object(tzIGeometricObject* obj_ptr) 
{
	object_ptr = obj_ptr;
	material_ptr = object_ptr->get_material();	
}

#endif
