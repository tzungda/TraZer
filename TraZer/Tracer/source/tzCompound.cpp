
#include <vector>

#include "../include/tzConstants.h"
#include "../include/tzCompound.h"
					

//===================================================================================
tzCompound::tzCompound(void)
	: 	tzIGeometricObject()
{}


//===================================================================================
tzCompound* tzCompound::clone(void) const 
{
	return (new tzCompound(*this));
}


//===================================================================================
tzCompound::tzCompound(const tzCompound& c)
	: tzIGeometricObject(c) 
{
	
	copyObjects(c.objects);					
}


//===================================================================================
tzCompound& tzCompound::operator= (const tzCompound& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);						
	
	copyObjects(rhs.objects);				

	return (*this);
}


//===================================================================================
tzCompound::~tzCompound(void)
{
	deleteObjects();				
}


//===================================================================================
void tzCompound::addObject(tzIGeometricObject* object_ptr)
{
	objects.push_back(object_ptr);	
}


//===================================================================================
void tzCompound::setMaterial(tzIMaterial* mMaterialPtr) 
{
	int num_objects = (int)objects.size();

	for (int j = 0; j < num_objects; j++)
		objects[j]->setMaterial(mMaterialPtr);
}


//===================================================================================
void tzCompound::deleteObjects(void) 
{
	int num_objects = (int)objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		delete objects[j];
		objects[j] = NULL;
	}	
	
	objects.erase(objects.begin(), objects.end());
}


//===================================================================================
void tzCompound::copyObjects(const std::vector<tzIGeometricObject*>& rhs_ojects) 
{
	deleteObjects();    	
	int num_objects = (int)rhs_ojects.size();
	
	for (int j = 0; j < num_objects; j++)
		objects.push_back(rhs_ojects[j]->clone());
}


//===================================================================================
bool tzCompound::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const 
{
	float		t;
	tzNormal		normal;
	tzPoint3D		local_hit_point;
	bool		hit 		= false;
				tmin 		= kHugeValue;
	int 		num_objects	= (int)objects.size();
	
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			mMaterialPtr	= objects[j]->getMaterial();	// lhs is GeometricObject::mMaterialPtr
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

//===================================================================================
bool tzCompound::shadowHit(const tzRay& ray, float& tmin) const {
	float		t;
	tzNormal		normal;
	tzPoint3D		local_hit_point;
	bool		hit = false;
	tmin = kHugeValue;
	int 		num_objects = (int)objects.size();

	for (int j = 0; j < num_objects; j++)
		if (objects[j]->shadowHit(ray, t) && (t < tmin)) {
			hit = true;
			tmin = t;
		}

	return (hit);
}

