#include "../Include/tzCoreCamera.h"
#include "../Include/tzConstants.h"
/*
Constructor/Destructor
*/

//===================================================================================
tzCoreCamera::tzCoreCamera()
{
	mFace.x = 0.0f, mFace.y = 0.0f, mFace.z = -1.0f;
	mUp.x = 0.0f, mUp.y = 1.0f, mUp.z = 0.0f;
	mRight.x = 1.0f, mRight.y = 0.0f, mRight.z = 0.0f;

	mAspect = 1.0f;
	mFov = 45.0f;

	mNearPlane = 0.0f;
	mFarPlane = 10000.0f;
}

//===================================================================================
tzCoreCamera::~tzCoreCamera()
{
}


/*
Interfaces
*/


//===================================================================================
tzVector3D tzCoreCamera::faceDir() const
{
	tzVector3D face = mFace*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	face.normalize();

	return face;
}

//===================================================================================
tzVector3D tzCoreCamera::upDir() const
{
	tzVector3D up = mUp*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	up.normalize();

	return up;
}

//===================================================================================
tzVector3D tzCoreCamera::rightDir() const
{
	tzVector3D right = mRight*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	right.normalize();

	return right;
}

//===================================================================================
float tzCoreCamera::aspect() const
{
	return mAspect;
}

//===================================================================================
float tzCoreCamera::fov() const
{
	return mFov;
}

//===================================================================================
float tzCoreCamera::nearPlane() const
{
	return mNearPlane;
}

//===================================================================================
float tzCoreCamera::farPlane() const
{
	return mFarPlane;
}

//===================================================================================
void tzCoreCamera::setAspect(float _aspect)
{
	mAspect = _aspect;
}

//===================================================================================
void tzCoreCamera::setFov( float _fov )
{
	mFov = _fov;
}

//===================================================================================
void tzCoreCamera::setNearPlane(float _nearPlane)
{
	mNearPlane = _nearPlane;
}

//===================================================================================
void tzCoreCamera::setFarPlane(float _farPlane)
{
	mFarPlane = _farPlane;
}

//===================================================================================
tzMatrix tzCoreCamera::perspectiveProjection() const
{
	//assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

	const float tanHalfFovy = tan( mFov * degreeToRadian / 2.0f);

	tzMatrix result;
	result.initializeWithAValue( 0.0f );
	result.m[0][0] = 1.0f/ (mAspect * tanHalfFovy);
	result.m[1][1] = 1.0f / (tanHalfFovy);
	result.m[2][2] = -(mFarPlane + mNearPlane) / (mFarPlane - mNearPlane);
	result.m[2][3] = -1.0f;
	result.m[3][2] = -(2.0f * mFarPlane * mNearPlane) / (mFarPlane - mNearPlane);
	return result;
}

tzMatrix tzCoreCamera::orthogonalProjection() const
{
	//-aspect * size, aspect * size, -size, size, nearVal, farVal
	//
	float testSize = 1.5f;
	//
	float left = -mAspect*testSize;
	float right = mAspect*testSize;
	float bottom = -testSize;
	float top = testSize;
	float zNear = mNearPlane;
	float zFar = mFarPlane;

	tzMatrix result;
	//result.initializeWithAValue(1.0f);
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = -2.0f / (zFar - zNear);
	result.m[3][0] = -(right + left) / (right - left);
	result.m[3][1] = -(top + bottom) / (top - bottom);
	result.m[3][2] = -(zFar + zNear) / (zFar - zNear);

	return result;

	/*
	tmat4x4<T, defaultp> Result(1);
		Result[0][0] = static_cast<T>(2) / (right - left);
		Result[1][1] = static_cast<T>(2) / (top - bottom);
		Result[2][2] = - static_cast<T>(2) / (zFar - zNear);
		Result[3][0] = - (right + left) / (right - left);
		Result[3][1] = - (top + bottom) / (top - bottom);
		Result[3][2] = - (zFar + zNear) / (zFar - zNear);
		return Result;
	*/
}

//===================================================================================
/*
tzVector3D tzCoreCamera::move(float deltaX, float deltaY, float deltaZ)
{
	tzVector3D up = upDir();
	tzVector3D face = faceDir();
	tzVector3D right = rightDir();
	mPosition += right*deltaX + up*deltaY + face*deltaZ;

	updateTransformMatrix();

	return mPosition;
}
*/
