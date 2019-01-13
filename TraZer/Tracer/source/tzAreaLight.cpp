
#include "../include/tzAreaLight.h"

//===================================================================================	
tzAreaLight::tzAreaLight(void)
	: 	tzILight(),
		object_ptr(NULL),
		mMaterialPtr(NULL)
{}	


//===================================================================================	
tzAreaLight::tzAreaLight(const tzAreaLight& al)
	: 	tzILight(al) 
{
	if(al.object_ptr)
		object_ptr = al.object_ptr->clone(); 
	else  object_ptr = NULL;
	
	if(al.mMaterialPtr)
		mMaterialPtr = al.mMaterialPtr->clone(); 
	else  mMaterialPtr = NULL;
}


//===================================================================================
tzILight* tzAreaLight::clone(void) const {
	return (new tzAreaLight(*this));
}					


//===================================================================================								
tzAreaLight::~tzAreaLight(void) 
{
	/*
	if (object_ptr) 
	{
		delete object_ptr;
		object_ptr = NULL;
	}
	*/
	/*
	if (mMaterialPtr) {
		delete mMaterialPtr;
		mMaterialPtr = NULL;
	}
	*/
}


//===================================================================================
tzAreaLight& tzAreaLight::operator= (const tzAreaLight& rhs) {
	if (this == &rhs)
		return (*this);
		
	tzILight::operator=(rhs);
	
	if (object_ptr) {
		delete object_ptr;
		object_ptr = NULL;
	}

	if (rhs.object_ptr)
		object_ptr = rhs.object_ptr->clone();
		
	if (mMaterialPtr) {
		delete mMaterialPtr;
		mMaterialPtr = NULL;
	}

	if (rhs.mMaterialPtr)
		mMaterialPtr = rhs.mMaterialPtr->clone();

	return (*this);
}


//===================================================================================
tzVector3D tzAreaLight::getDirection(tzShadeRec& sr)
{
	sample_point = object_ptr->sample();    // used in the G function
	light_normal = object_ptr->getNormal(sample_point); 
	wi = sample_point - sr.mHitPoint;  		// used in the G function
	wi.normalize();
	
	return (wi);
}


//===================================================================================
tzRGBColor tzAreaLight::L(tzShadeRec& sr)
{
	float ndotd = (float)( -light_normal * wi ); 
	
	if (ndotd > 0.0)		
		return (mMaterialPtr->getLe(sr)); 
	else
		return (black);
}


//===================================================================================
bool tzAreaLight::inShadow(const tzRay& ray, const tzShadeRec& sr) const 
{
	float t;
	int num_objects = (int)sr.mWorld.mObjects.size();
	float ts = (float)((sample_point - ray.o) * ray.d);
	
	for (int j = 0; j < num_objects; j++)
	{
		if (sr.mWorld.mObjects[j]->shadowHit(ray, t) && t < ts)
		{
			return (true); 
		}
	}

	return (false);     
}


//===================================================================================
float tzAreaLight::G(const tzShadeRec& sr) const 
{
	float ndotd = (float)(-light_normal * wi);
	float d2 	= (float)sample_point.d_squared(sr.mHitPoint);
		
	return (ndotd / d2);
}


//===================================================================================
float tzAreaLight::pdf(const tzShadeRec& sr) const 
{
	return (object_ptr->pdf(sr));
}

