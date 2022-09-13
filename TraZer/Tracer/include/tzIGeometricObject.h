#ifndef TZ_GEOMETRIC_INTERFACE
#define TZ_GEOMETRIC_INTERFACE


#include <math.h>  // a lot of hit functions use use maths functions

#include "../include/tzConstants.h"
#include "tzBBox.h"
#include "tzColor.h"
#include "tzPoint3D.h"
#include "tzVector3D.h"
#include "tzNormal.h"
#include "../include/tzRay.h"
#include "../include/tzShadeRec.h"
#include <memory>
#include <vector>

class tzIMaterial;	
class tzITexture;

class tzIGeometricObject {	
	public:	

		tzIGeometricObject(void);									
		
		tzIGeometricObject(const tzIGeometricObject& object);			
	
		virtual std::shared_ptr<tzIGeometricObject> clone(void) const = 0;

		virtual 												
		~tzIGeometricObject(void);	
			
		virtual bool hit(const tzRay& ray, float& t, tzShadeRec& s)  = 0;

		//
		virtual bool shadowHit(const tzRay &ray, const tzShadeRec& sr, float &tmin) const;
		
		virtual void  setMaterial(std::shared_ptr < tzIMaterial > mPtr, int threadId);
		virtual void  setAlphaTexture( std::shared_ptr<tzITexture> alphaTexture );
		
		virtual std::shared_ptr < tzIMaterial > getMaterial( int threadId ) const;

				   
		// The following three functions are only required for Chapter 3
		
		void setColor(const tzColor& c);
				
		void setColor(const float r, const float g, const float b);
		
		tzColor getColor(void);
		
		virtual void setBoundingBox(void);
		
		virtual tzBBox getBoundingBox(void) const;

		virtual void addObject(std::shared_ptr<tzIGeometricObject> object_ptr);
				
		
		// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
		 
		virtual tzPoint3D sample(const tzShadeRec& sr );
		
		virtual float pdf(const tzShadeRec& sr); 
				
		
		// The following two functions allow us to simplify the code for smooth shaded triangle meshes
		
		virtual tzNormal getNormal(void) const; 
		
		virtual tzNormal getNormal(const tzPoint3D& p);

	
	protected:
	
		std::shared_ptr < tzIMaterial >   mMaterialPtr[MAX_THREADS];   	// mutable allows the const functions Compound::hit, Instance::hit, and RegularGrid::hit to assign to mMaterialPtr
		tzColor   		   mColor;				// only used for Bare Bones ray tracing

		std::shared_ptr<tzITexture>	   mAlphaTexture;
	
		tzIGeometricObject& operator= (const tzIGeometricObject& rhs);
};


//===================================================================================
inline void tzIGeometricObject::setColor(const tzColor& c) 
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
inline tzColor tzIGeometricObject::getColor(void) 
{
	return (mColor);
}

#endif
