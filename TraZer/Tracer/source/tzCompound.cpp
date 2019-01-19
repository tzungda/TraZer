
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
	
	copyObjects(c.mObjects);
}


//===================================================================================
tzCompound& tzCompound::operator= (const tzCompound& rhs) 
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator= (rhs);						
	
	copyObjects(rhs.mObjects);

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
	mObjects.push_back(object_ptr);
}


//===================================================================================
void tzCompound::setMaterial(tzIMaterial* mMaterialPtr) 
{
	int numObjects = (int)mObjects.size();

	for (int j = 0; j < numObjects; j++)
		mObjects[j]->setMaterial(mMaterialPtr);
}


//===================================================================================
void tzCompound::deleteObjects(void) 
{
	int numObjects = (int)mObjects.size();
	
	for (int j = 0; j < numObjects; j++)
	{
		delete mObjects[j];
		mObjects[j] = NULL;
	}	
	
	mObjects.erase(mObjects.begin(), mObjects.end());
}


//===================================================================================
void tzCompound::copyObjects(const std::vector<tzIGeometricObject*>& rhsOjects) 
{
	deleteObjects();    	
	int numObjects = (int)rhsOjects.size();
	
	for (int j = 0; j < numObjects; j++)
		mObjects.push_back(rhsOjects[j]->clone());
}


//===================================================================================
bool tzCompound::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const 
{
	float		t;
	tzNormal		normal;
	tzPoint3D		local_hit_point;
	bool		hit 		= false;
				tmin 		= kHugeValue;
	int 		numObjects	= (int)mObjects.size();
	
	for (int j = 0; j < numObjects; j++)
		if (mObjects[j]->hit(ray, t, sr) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			mMaterialPtr	= mObjects[j]->getMaterial();	// lhs is GeometricObject::mMaterialPtr
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
	int 		numObjects = (int)mObjects.size();

	for (int j = 0; j < numObjects; j++)
		if (mObjects[j]->shadowHit(ray, t) && (t < tmin)) {
			hit = true;
			tmin = t;
		}

	return (hit);
}

