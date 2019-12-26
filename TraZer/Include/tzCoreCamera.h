#ifndef TZ_CORE_CAMERA
#define TZ_CORE_CAMERA

/*
 This class is for the triangulated object
*/

#include "../Include/tzCoreObject.h"
#include "../Include/tzCoreTransform.h"

#include <vector>
#include "tzPoint3D.h"
#include "tzNormal.h"


class tzCoreCamera : public tzCoreObject,
				   public tzCoreTransform
{
public:
	tzCoreCamera( );
	virtual ~tzCoreCamera();

	

private: // member data
	tzVector3D	mFace;
	tzVector3D	mUp;
	tzVector3D	mRight;

	float		mAspect;
	float		mFov;
	float		mNearPlane;
	float		mFarPlane;


public: // 

	tzVector3D	faceDir() const;
	tzVector3D	upDir() const;
	tzVector3D	rightDir() const;
	float		aspect( ) const;
	float		fov( ) const;
	float		nearPlane( ) const;
	float		farPlane() const;
	void		setAspect( float aspect );
	void		setFov( float fov);
	void		setNearPlane( float nearPlane);
	void		setFarPlane(float farPlane);
	tzMatrix	perspectiveProjection( ) const;
	tzMatrix	orthogonalProjection( ) const;

	//
	//virtual tzVector3D	move(float deltaX, float deltaY, float deltaZ);
	
};

#endif
