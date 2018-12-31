
#include <vector>

#include "../include/tzConstants.h"
#include "../include/tzCompound.h"
					

// ----------------------------------------------------------------  default constructor

tzCompound::tzCompound(void)
	: 	tzIGeometricObject()
{}


// ---------------------------------------------------------------- clone

tzCompound*
tzCompound::clone(void) const {
	return (new tzCompound(*this));
}


// ---------------------------------------------------------------- copy constructor

tzCompound::tzCompound(const tzCompound& c)
	: tzIGeometricObject(c) {
	
	copy_objects(c.objects);					
}



// ---------------------------------------------------------------- assignment operator

tzCompound&
tzCompound::operator= (const tzCompound& rhs) {
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);						
	
	copy_objects(rhs.objects);				

	return (*this);
}


// ---------------------------------------------------------------- destructor

tzCompound::~tzCompound(void) {
	delete_objects();				
}


// ---------------------------------------------------------------- add_object

void 
tzCompound::add_object(tzIGeometricObject* object_ptr) {
	objects.push_back(object_ptr);	
}


//------------------------------------------------------------------ set_material
// sets the same material on all objects

void 
tzCompound::set_material(tzIMaterial* material_ptr) {
	int num_objects = (int)objects.size();

	for (int j = 0; j < num_objects; j++)
		objects[j]->set_material(material_ptr);
}


//------------------------------------------------------------------ delete_objects
// Deletes the objects in the objects array, and erases the array.
// The array still exists, because it'ss an automatic variable, but it's empty 

void
tzCompound::delete_objects(void) {
	int num_objects = (int)objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		delete objects[j];
		objects[j] = NULL;
	}	
	
	objects.erase(objects.begin(), objects.end());
}


//------------------------------------------------------------------ copy_objects

void
tzCompound::copy_objects(const std::vector<tzIGeometricObject*>& rhs_ojects) {
	delete_objects();    	
	int num_objects = (int)rhs_ojects.size();
	
	for (int j = 0; j < num_objects; j++)
		objects.push_back(rhs_ojects[j]->clone());
}


//------------------------------------------------------------------ hit

bool 															 
tzCompound::hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const {
	double		t; 
	tzNormal		normal;
	tzPoint3D		local_hit_point;
	bool		hit 		= false;
				tmin 		= kHugeValue;
	int 		num_objects	= (int)objects.size();
	
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			material_ptr	= objects[j]->get_material();	// lhs is GeometricObject::material_ptr
			normal			= sr.mNormal;
			local_hit_point	= sr.mLocalHitPoint;  
		}
	
	if (hit) {
		sr.mT				= (float)tmin;
		sr.mNormal 			= normal;   
		sr.mLocalHitPoint 	= local_hit_point;  
	}
	
	return (hit);
}


bool tzCompound::shadow_hit(const tzRay& ray, float& tmin) const {
	float		t;
	tzNormal		normal;
	tzPoint3D		local_hit_point;
	bool		hit = false;
	tmin = kHugeValue;
	int 		num_objects = (int)objects.size();

	for (int j = 0; j < num_objects; j++)
		if (objects[j]->shadow_hit(ray, t) && (t < tmin)) {
			hit = true;
			tmin = t;
		}

	return (hit);
}

