#include "../Include/tzCoreLight.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreLight::tzCoreLight(): mIntensity( 1.0f ), mShadowmapResolution(1024), mNearPlane(0.1f), mFarPlane(50.0f), mLightType( directional )
{
}

//===================================================================================
tzCoreLight::~tzCoreLight()
{
}

/*
interfaces
*/

//===================================================================================
void tzCoreLight::setColor(const tzVector3D &color)
{
	mColor = color;
}

//===================================================================================
void tzCoreLight::setIntensity(float intensity)
{
	mIntensity = intensity;
}

//===================================================================================
void tzCoreLight::setShadowmapResolution(int shadowmapRes)
{
	mShadowmapResolution = shadowmapRes;
}

//===================================================================================
void tzCoreLight::setNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

//===================================================================================
void tzCoreLight::setFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

//===================================================================================
void tzCoreLight::setLightType(tzLightType lightType)
{
	mLightType = lightType;
}

//===================================================================================
tzVector3D tzCoreLight::color() const
{
	return mColor;
}

//===================================================================================
float tzCoreLight::intensity() const
{
	return mIntensity;
}

//===================================================================================
int tzCoreLight::shadowmapResolution() const
{
	return mShadowmapResolution;
}

//===================================================================================
float tzCoreLight::nearPlane() const
{
	return mNearPlane;
}

//===================================================================================
float tzCoreLight::farPlane() const
{
	return mFarPlane;
}

//===================================================================================
tzLightType tzCoreLight::lightType() const
{
	return mLightType;
}

