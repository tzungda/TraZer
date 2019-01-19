
#include "../include/tzAreaLight.h"

//===================================================================================	
tzAreaLight::tzAreaLight(void)
	: 	tzILight(),
		mObjectPtr(NULL),
		mMaterialPtr(NULL)
{}	


//===================================================================================	
tzAreaLight::tzAreaLight(const tzAreaLight& al)
	: 	tzILight(al) 
{
	if(al.mObjectPtr)
		mObjectPtr = al.mObjectPtr->clone();
	else  mObjectPtr = NULL;
	
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
	
	if (mObjectPtr) {
		delete mObjectPtr;
		mObjectPtr = NULL;
	}

	if (rhs.mObjectPtr)
		mObjectPtr = rhs.mObjectPtr->clone();
		
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
	mSamplePoint[sr.mThreadId] = mObjectPtr->sample( sr );    // used in the G function
	mLightNormal[sr.mThreadId] = mObjectPtr->getNormal(mSamplePoint[sr.mThreadId]);
	mWi[sr.mThreadId] = mSamplePoint[sr.mThreadId] - sr.mHitPoint;  		// used in the G function
	mWi[sr.mThreadId].normalize();
	
	return (mWi[sr.mThreadId]);
}


//===================================================================================
tzColor tzAreaLight::L(tzShadeRec& sr)
{
	float ndotd = (float)( -mLightNormal[sr.mThreadId] * mWi[sr.mThreadId]);
	
	if (ndotd > 0.0)		
		return (mMaterialPtr->getLe(sr)); 
	else
		return (black);
}


//===================================================================================
bool tzAreaLight::inShadow(const tzRay& ray, const tzShadeRec& sr) const 
{
	float t;
	int numObjects = (int)sr.mWorld.mObjects.size();
	float ts = (float)((mSamplePoint[sr.mThreadId] - ray.mOrigin) * ray.mDirection);
	
	for (int j = 0; j < numObjects; j++)
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
	float ndotd = (float)(-mLightNormal[sr.mThreadId] * mWi[sr.mThreadId]);
	float d2 	= (float)mSamplePoint[sr.mThreadId].dSquared(sr.mHitPoint);
		
	return (ndotd / d2);
}


//===================================================================================
float tzAreaLight::pdf(const tzShadeRec& sr) const 
{
	return (mObjectPtr->pdf(sr));
}

