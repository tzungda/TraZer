#ifndef TZ_GEOMETRIC_INTERFACE
#define TZ_GEOMETRIC_INTERFACE


#include <math.h>  // a lot of hit functions use use maths functions

#include "../include/tzConstants.h"
#include "tzBBox.h"
#include "tzRGBColor.h"
#include "tzPoint3D.h"
#include "tzVector3D.h"
#include "tzNormal.h"
#include "../include/tzRay.h"
#include "../include/tzShadeRec.h"

class tzIMaterial;	


class tzIGeometricObject {	
	public:	

		tzIGeometricObject(void);									
		
		tzIGeometricObject(const tzIGeometricObject& object);			
	
		virtual tzIGeometricObject*								
		clone(void) const = 0;

		virtual 												
		~tzIGeometricObject(void);	
			
		virtual bool hit(const tzRay& ray, float& t, tzShadeRec& s) const = 0;

		//
		virtual bool shadowHit(const tzRay &ray, float &tmin) const;
		
		virtual void  setMaterial(tzIMaterial* mPtr); 
		
		tzIMaterial* getMaterial(void) const;

				   
		// The following three functions are only required for Chapter 3
		
		void setColor(const tzRGBColor& c);
				
		void setColor(const float r, const float g, const float b);
		
		tzRGBColor getColor(void);
		
		virtual void setBoundingBox(void);
		
		virtual tzBBox getBoundingBox(void);

		virtual void addObject(tzIGeometricObject* object_ptr);
				
		
		// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
		 
		virtual tzPoint3D sample(void);
		
		virtual float pdf(const tzShadeRec& sr); 
				
		
		// The following two functions allow us to simplify the code for smooth shaded triangle meshes
		
		virtual tzNormal getNormal(void) const; 
		
		virtual tzNormal getNormal(const tzPoint3D& p);

	
	protected:
	
		mutable tzIMaterial*   mMaterialPtr;   	// mutable allows the const functions Compound::hit, Instance::hit, and RegularGrid::hit to assign to mMaterialPtr
		tzRGBColor   		   mColor;				// only used for Bare Bones ray tracing
	
		tzIGeometricObject& operator= (const tzIGeometricObject& rhs);
};


//===================================================================================
inline void tzIGeometricObject::setColor(const tzRGBColor& c) 
{
	mColor = c;
}

//===================================================================================
inline void tzIGeometricObject::setColor(const float r, const float g, const float b) 
{
	mColor.r = r;
	mColor.b = b;
	mColor.g = g;
}

//===================================================================================
inline tzRGBColor tzIGeometricObject::getColor(void) 
{
	return (mColor);
}

#endif
