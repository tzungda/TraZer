#ifndef TZ_TRACER_AREA_LIGHT
#define TZ_TRACER_AREA_LIGHT


#include "tzColor.h"
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
		
		void  setObject(tzIGeometricObject* obj_ptr);
		
		virtual tzVector3D getDirection( tzShadeRec& s) ;
		
		virtual tzColor L( tzShadeRec& sr) ;
		
		virtual bool inShadow(const tzRay& ray, const tzShadeRec& sr) const;
		
		virtual float G(const tzShadeRec& sr) const;
		
		virtual float pdf(const tzShadeRec& sr) const;
		
	private:
		
		tzIGeometricObject* 	object_ptr;
		tzIMaterial* 			mMaterialPtr;	 // will be an emissive material
		tzPoint3D				samplePoint[MAX_THREADS];
		tzNormal				light_normal[MAX_THREADS];    // assigned in get_direction - which therefore can't be const for any light
		tzVector3D				wi[MAX_THREADS];			     // unit direction from hit point being shaded to sample point on light surface			
	
};


//===================================================================================
inline void tzAreaLight::setObject(tzIGeometricObject* obj_ptr) 
{
	object_ptr = obj_ptr;
	mMaterialPtr = object_ptr->getMaterial();	
}

#endif
