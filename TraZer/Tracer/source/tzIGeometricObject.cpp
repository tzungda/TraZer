
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

tzNormal
tzIGeometricObject::get_normal(void) const{
	return (tzNormal());
} 


// ----------------------------------------------------------------------- set_material

void 
tzIGeometricObject::set_material(tzIMaterial* mPtr) {
	material_ptr = mPtr;
}


//===================================================================================
bool tzIGeometricObject::shadowHit(const tzRay &ray, float &tmin) const
{
	return false;
}

// ----------------------------------------------------------------------- get_material

tzIMaterial*
tzIGeometricObject::get_material(void) const {
	return (material_ptr);
}


// ----------------------------------------------------------------------- compute_normal

tzNormal
tzIGeometricObject::get_normal(const tzPoint3D& p) {
	return (tzNormal());
}  


// ----------------------------------------------------------------------- set_bounding_box

void 
tzIGeometricObject::set_bounding_box (void) {}


// ----------------------------------------------------------------------- set_bounding_box

tzBBox 
tzIGeometricObject::get_bounding_box (void) {
	return tzBBox();
}



// ----------------------------------------------------------------------- sample
// returns a sample point on the object for area light shading

tzPoint3D
tzIGeometricObject::sample(void) {
	return (tzPoint3D(0.0));
}


// ----------------------------------------------------------------------- pdf
// returns the probability density function for area light shading
		
float
tzIGeometricObject::pdf(const tzShadeRec& sr) {
	return (1.0);
}  	

