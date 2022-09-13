
#include "../include/tzConstants.h"
#include "../include/tzIMaterial.h"
#include "../include/tzIGeometricObject.h"
#include "../include/tzITexture.h"

//===================================================================================
tzIGeometricObject::tzIGeometricObject(void)
	: mColor(black),
		mAlphaTexture(nullptr)
		//shadows(true)
{}


//===================================================================================
tzIGeometricObject::tzIGeometricObject (const tzIGeometricObject& object)
	: mColor(object.mColor)
		/*shadows(object.shadows)*/ 
{
	for ( int i = 0; i < MAX_THREADS; i++)
	{
		if(object.mMaterialPtr[i])
			mMaterialPtr[i] = object.mMaterialPtr[i]->clone(); 
		else  
			mMaterialPtr[i] = nullptr;
	}

	if ( object.mAlphaTexture )
	{
		mAlphaTexture = object.mAlphaTexture;
	}
	else
	{
		mAlphaTexture = nullptr;
	}
}	


//===================================================================================
tzIGeometricObject& tzIGeometricObject::operator= (const tzIGeometricObject& rhs) 
{
	if (this == &rhs)
		return (*this);
		
	mColor = rhs.mColor;

	for (int i = 0; i < MAX_THREADS; i++)
	{
		if (rhs.mMaterialPtr[i])
			mMaterialPtr[i] = rhs.mMaterialPtr[i]->clone();
	}

	//shadows = rhs.shadows;

	return (*this);
}


//===================================================================================
tzIGeometricObject::~tzIGeometricObject (void) 
{	
}


//===================================================================================
void tzIGeometricObject::addObject(std::shared_ptr<tzIGeometricObject> object_ptr)
{
}


//===================================================================================
tzNormal tzIGeometricObject::getNormal(void) const
{
	return (tzNormal());
} 

//===================================================================================
void tzIGeometricObject::setMaterial(std::shared_ptr < tzIMaterial> mPtr, int threadId)
{
	if (threadId == ~0 )
	{
		for ( int i = 0; i < MAX_THREADS; i++ )
		{
			mMaterialPtr[i] = mPtr;
		}
	}
	else
	{
		mMaterialPtr[threadId] = mPtr;
	}
}

//===================================================================================
void tzIGeometricObject::setAlphaTexture( std::shared_ptr<tzITexture> alphaTexture)
{
	this->mAlphaTexture = alphaTexture;
}

//===================================================================================
bool tzIGeometricObject::shadowHit(const tzRay &ray, const tzShadeRec& sr, float &tmin) const
{
	return false;
}

//===================================================================================
std::shared_ptr < tzIMaterial> tzIGeometricObject::getMaterial(int threadId) const
{
	return mMaterialPtr[threadId];
}


//===================================================================================
tzNormal tzIGeometricObject::getNormal(const tzPoint3D& p) 
{
	return (tzNormal());
}  


//===================================================================================
void tzIGeometricObject::setBoundingBox (void) {}


//===================================================================================
tzBBox tzIGeometricObject::getBoundingBox (void) const
{
	return tzBBox();
}


//===================================================================================
tzPoint3D tzIGeometricObject::sample(const tzShadeRec& sr)
{
	return (tzPoint3D(0.0));
}


//===================================================================================		
float
tzIGeometricObject::pdf(const tzShadeRec& sr)
{
	return (1.0);
}  	

