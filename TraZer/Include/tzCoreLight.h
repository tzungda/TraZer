#ifndef TZ_CORE_LIGHT
#define TZ_CORE_LIGHT

/*
 This class is for the triangulated object
*/

#include "../Include/tzCoreObject.h"
#include "../Include/tzCoreTransform.h"
#include "tzCoreTypes.h"
#include <vector>

class tzCoreLight : public tzCoreObject,
				   public tzCoreTransform
{
public:
	tzCoreLight( );
	virtual ~tzCoreLight();
	
private: // member data
	tzVector3D		mColor;
	float			mIntensity;

	int				mShadowmapResolution;
	float			mNearPlane;
	float			mFarPlane;
	tzLightType		mLightType;

public: // 

	// set data interfaces
	void							setColor( const tzVector3D &color );
	void							setIntensity( float intensity );
	void							setShadowmapResolution( int shadowmapRes );
	void							setNearPlane( float nearPlane );
	void							setFarPlane( float farPlane );
	void							setLightType(tzLightType lightType );
	
	// get data interfaces
	tzVector3D						color( ) const;
	float							intensity( ) const;
	int								shadowmapResolution() const;
	float							nearPlane() const;
	float							farPlane() const;
	tzLightType						lightType() const;
};

#endif
