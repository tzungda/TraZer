#ifndef TZ_GEOMETRIC_INTERFACE
#define TZ_GEOMETRIC_INTERFACE


#include <math.h>  // a lot of hit functions use use maths functions

#include "../include/tzConstants.h"
#include "BBox.h"
#include "RGBColor.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Normal.h"
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
		set_color(const RGBColor& c);
				
		void
		set_color(const float r, const float g, const float b);
		
		RGBColor
		get_color(void);
		
		virtual void 
		set_bounding_box(void);
		
		virtual BBox 
		get_bounding_box(void);

		virtual void 										// required for compound objects
		add_object(tzIGeometricObject* object_ptr);
				
		
		// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
		 
		virtual Point3D 		
		sample(void);
		
		virtual float pdf(const tzShadeRec& sr); 
				
		
		// The following two functions allow us to simplify the code for smooth shaded triangle meshes
		
		virtual Normal
		get_normal(void) const; 
		
		virtual Normal
		get_normal(const Point3D& p); 

	
	protected:
	
		/*mutable*/ tzIMaterial*   material_ptr;   	// mutable allows the const functions Compound::hit, Instance::hit, and RegularGrid::hit to assign to material_ptr
		RGBColor   			color;				// only used for Bare Bones ray tracing
	
		tzIGeometricObject&						
		operator= (const tzIGeometricObject& rhs);
};


// --------------------------------------------------------------------  set_color

inline void
tzIGeometricObject::set_color(const RGBColor& c) {
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

inline RGBColor 
tzIGeometricObject::get_color(void) {
	return (color);
}

#endif
