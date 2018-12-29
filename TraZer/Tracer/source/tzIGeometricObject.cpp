
#include "../include/tzConstants.h"
#include "../include/tzIMaterial.h"
#include "../include/tzIGeometricObject.h"


// ---------------------------------------------------------------------- default constructor

tzIGeometricObject::tzIGeometricObject(void)
	: 	color(black),
		material_ptr(NULL)
		//shadows(true)
{}


// ---------------------------------------------------------------------- copy constructor

tzIGeometricObject::tzIGeometricObject (const tzIGeometricObject& object)
	: 	color(object.color)
		/*shadows(object.shadows)*/ 
{
	if(object.material_ptr)
		material_ptr = object.material_ptr->clone(); 
	else  
		material_ptr = NULL;
}	


// ---------------------------------------------------------------------- assignment operator

tzIGeometricObject&														
tzIGeometricObject::operator= (const tzIGeometricObject& rhs) {
	if (this == &rhs)
		return (*this);
		
	color = rhs.color;
	
	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}

	if (rhs.material_ptr)
		material_ptr = rhs.material_ptr->clone();

	//shadows = rhs.shadows;

	return (*this);
}


// ---------------------------------------------------------------------- destructor

tzIGeometricObject::~tzIGeometricObject (void) 
{	
	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}
}


// ---------------------------------------------------------------------- add_object
// required for Compound objects 

void 												
tzIGeometricObject::add_object(tzIGeometricObject* object_ptr) {}


// ----------------------------------------------------------------------- get_normal

Normal
tzIGeometricObject::get_normal(void) const{
	return (Normal());
} 


// ----------------------------------------------------------------------- set_material

void 
tzIGeometricObject::set_material(tzIMaterial* mPtr) {
	material_ptr = mPtr;
}


//===================================================================================
bool tzIGeometricObject::shadow_hit(const tzRay &ray, float &tmin) const
{
	return false;
}

// ----------------------------------------------------------------------- get_material

tzIMaterial*
tzIGeometricObject::get_material(void) const {
	return (material_ptr);
}


// ----------------------------------------------------------------------- compute_normal

Normal
tzIGeometricObject::get_normal(const Point3D& p) {
	return (Normal());
}  


// ----------------------------------------------------------------------- set_bounding_box

void 
tzIGeometricObject::set_bounding_box (void) {}


// ----------------------------------------------------------------------- set_bounding_box

BBox 
tzIGeometricObject::get_bounding_box (void) {
	return BBox();
}



// ----------------------------------------------------------------------- sample
// returns a sample point on the object for area light shading

Point3D 
tzIGeometricObject::sample(void) {
	return (Point3D(0.0));
}


// ----------------------------------------------------------------------- pdf
// returns the probability density function for area light shading
		
float
tzIGeometricObject::pdf(const tzShadeRec& sr) {
	return (1.0);
}  	

