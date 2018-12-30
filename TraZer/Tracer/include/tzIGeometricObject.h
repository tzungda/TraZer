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
			
		virtual bool hit(const tzRay& ray, double& t, tzShadeRec& s) const = 0;

		//
		virtual bool shadow_hit(const tzRay &ray, float &tmin) const;
		
		virtual void 							// This needs to be virtual so that it can be overridden in Compound
		set_material(tzIMaterial* mPtr); 			// It therefore shouldn't be inlined
		
		tzIMaterial*					
		get_material(void) const;

				   
		// The following three functions are only required for Chapter 3
		
		void
		set_color(const tzRGBColor& c);
				
		void
		set_color(const float r, const float g, const float b);
		
		tzRGBColor
		get_color(void);
		
		virtual void 
		set_bounding_box(void);
		
		virtual tzBBox 
		get_bounding_box(void);

		virtual void 										// required for compound objects
		add_object(tzIGeometricObject* object_ptr);
				
		
		// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
		 
		virtual tzPoint3D
		sample(void);
		
		virtual float pdf(const tzShadeRec& sr); 
				
		
		// The following two functions allow us to simplify the code for smooth shaded triangle meshes
		
		virtual tzNormal
		get_normal(void) const; 
		
		virtual tzNormal
		get_normal(const tzPoint3D& p);

	
	protected:
	
		/*mutable*/ tzIMaterial*   material_ptr;   	// mutable allows the const functions Compound::hit, Instance::hit, and RegularGrid::hit to assign to material_ptr
		tzRGBColor   			color;				// only used for Bare Bones ray tracing
	
		tzIGeometricObject&						
		operator= (const tzIGeometricObject& rhs);
};


// --------------------------------------------------------------------  set_color

inline void
tzIGeometricObject::set_color(const tzRGBColor& c) {
	color = c;
}

// --------------------------------------------------------------------  set_color

inline void 
tzIGeometricObject::set_color(const float r, const float g, const float b) {
	color.r = r;
	color.b = b;
	color.g = g;
}

// --------------------------------------------------------------------  get_color

inline tzRGBColor
tzIGeometricObject::get_color(void) {
	return (color);
}

#endif
