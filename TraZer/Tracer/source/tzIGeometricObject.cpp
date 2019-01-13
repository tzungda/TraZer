
#include "../include/tzConstants.h"
#include "../include/tzIMaterial.h"
#include "../include/tzIGeometricObject.h"


//===================================================================================
tzIGeometricObject::tzIGeometricObject(void)
	: mColor(black),
		mMaterialPtr(NULL)
		//shadows(true)
{}


//===================================================================================
tzIGeometricObject::tzIGeometricObject (const tzIGeometricObject& object)
	: mColor(object.mColor)
		/*shadows(object.shadows)*/ 
{
	if(object.mMaterialPtr)
		mMaterialPtr = object.mMaterialPtr->clone(); 
	else  
		mMaterialPtr = NULL;
}	


//===================================================================================
tzIGeometricObject& tzIGeometricObject::operator= (const tzIGeometricObject& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	mColor = rhs.mColor;
	
	if (mMaterialPtr) {
		delete mMaterialPtr;
		mMaterialPtr = NULL;
	}

	if (rhs.mMaterialPtr)
		mMaterialPtr = rhs.mMaterialPtr->clone();

	//shadows = rhs.shadows;

	return (*this);
}


//===================================================================================
tzIGeometricObject::~tzIGeometricObject (void) 
{	
	if (mMaterialPtr) {
		delete mMaterialPtr;
		mMaterialPtr = NULL;
	}
}


//===================================================================================
void tzIGeometricObject::addObject(tzIGeometricObject* object_ptr) 
{
}


//===================================================================================
tzNormal tzIGeometricObject::getNormal(void) const
{
	return (tzNormal());
} 

//===================================================================================
void tzIGeometricObject::setMaterial(tzIMaterial* mPtr) 
{
	mMaterialPtr = mPtr;
}


//===================================================================================
bool tzIGeometricObject::shadowHit(const tzRay &ray, float &tmin) const
{
	return false;
}

//===================================================================================
tzIMaterial* tzIGeometricObject::getMaterial(void) const 
{
	return (mMaterialPtr);
}


//===================================================================================
tzNormal tzIGeometricObject::getNormal(const tzPoint3D& p) 
{
	return (tzNormal());
}  


//===================================================================================
void tzIGeometricObject::setBoundingBox (void) {}


//===================================================================================
tzBBox tzIGeometricObject::getBoundingBox (void) 
{
	return tzBBox();
}


//===================================================================================
tzPoint3D tzIGeometricObject::sample(void) 
{
	return (tzPoint3D(0.0));
}


//===================================================================================		
float
tzIGeometricObject::pdf(const tzShadeRec& sr)
{
	return (1.0);
}  	

