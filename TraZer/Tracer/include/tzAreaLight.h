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

		virtual std::shared_ptr<tzILight> clone(void) const;
		
		virtual ~tzAreaLight(void);
		
		tzAreaLight& operator= (const tzAreaLight& rhs);
		
		void  setObject(std::shared_ptr<tzIGeometricObject>  obj_ptr, int threadId);
		
		virtual tzVector3D getDirection( tzShadeRec& s) ;
		
		virtual tzColor L( tzShadeRec& sr) ;
		
		virtual bool inShadow(const tzRay& ray, const tzShadeRec& sr) const;
		
		virtual float G(const tzShadeRec& sr) const;
		
		virtual float pdf(const tzShadeRec& sr) const;
		
	private:
		
		std::shared_ptr<tzIGeometricObject> 	mObjectPtr;
		std::shared_ptr < tzIMaterial> 			mMaterialPtr;	 // will be an emissive material
		tzPoint3D				mSamplePoint[MAX_THREADS];
		tzNormal				mLightNormal[MAX_THREADS];    // assigned in get_direction - which therefore can't be const for any light
		tzVector3D				mWi[MAX_THREADS];			     // unit direction from hit point being shaded to sample point on light surface			
	
};


//===================================================================================
inline void tzAreaLight::setObject(std::shared_ptr<tzIGeometricObject>  objPtr, int threadId)
{
	mObjectPtr = objPtr;
	mMaterialPtr = mObjectPtr->getMaterial(threadId);
}

#endif
