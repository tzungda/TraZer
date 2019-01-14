#include "../Include/tzCoreTransform.h"
#include "../Include/tzConstants.h"

//===================================================================================
tzCoreTransform::tzCoreTransform()
{
	mTransform.setIdentity();
}

//===================================================================================
tzCoreTransform::tzCoreTransform(const tzMatrix &m)
{
	mTransform = m;
}

//===================================================================================
tzCoreTransform::tzCoreTransform(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	mTransform = tzMatrix(	m00, m01, m02, m03, m10, m11, m12, m13,
							m20, m21, m22, m23, m30, m31, m32, m33);
}

//===================================================================================
tzCoreTransform::~tzCoreTransform()
{
}

//===================================================================================
tzMatrix tzCoreTransform::rotateAlongVector(float angle, const tzVector3D &v)
{
	float a = angle*degreeToRadian;
	tzVector3D u = v;//normalize( v );
	u.normalize();

	//
	tzMatrix m(cos(a) + u.x*u.x*(1.0f - cos(a)), u.y*u.x*(1.0f - cos(a)) + u.z*sin(a), u.z*u.x*(1.0f - cos(a)) - u.y*sin(a), 0.0f,
		u.x*u.y*(1.0f - cos(a)) - u.z*sin(a), cos(a) + u.y*u.y*(1.0f - cos(a)), u.z*u.y*(1.0f - cos(a)) + u.x*sin(a), 0.0f,
		u.x*u.z*(1.0f - cos(a)) + u.y*sin(a), u.y*u.z*(1.0f - cos(a)) - u.x*sin(a), cos(a) + u.z*u.z*(1.0f - cos(a)), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

//===================================================================================
tzMatrix  tzCoreTransform::roatateX(float deltaAngle)
{
	mXAxisAngle += deltaAngle;

	mXRotationMatrix = tzMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(mXAxisAngle*degreeToRadian), sin(mXAxisAngle*degreeToRadian), 0.0f,
		0.0f, -sin(mXAxisAngle*degreeToRadian), cos(mXAxisAngle*degreeToRadian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mXRotationMatrix;
}

//===================================================================================
tzMatrix  tzCoreTransform::roatateY(float deltaAngle)
{
	mYAxisAngle += deltaAngle;

	mYRotationMatrix = tzMatrix(cos(mYAxisAngle*degreeToRadian), 0.0f, -sin(mYAxisAngle*degreeToRadian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(mYAxisAngle*degreeToRadian), 0.0f, cos(mYAxisAngle*degreeToRadian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mYRotationMatrix;
}

//===================================================================================
tzMatrix  tzCoreTransform::roatateZ(float deltaAngle)
{
	mZAxisAngle += deltaAngle;
 
	mZRotationMatrix = tzMatrix(cos(mZAxisAngle*degreeToRadian), sin(mZAxisAngle*degreeToRadian), 0.0f, 0.0f,
		-sin(mZAxisAngle*degreeToRadian), cos(mZAxisAngle*degreeToRadian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mZRotationMatrix;
}

//===================================================================================
tzMatrix  tzCoreTransform::updateTransformMatrix()
{
	mTransform = mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	mTransform.m[3][0] = mPosition.x;
	mTransform.m[3][1] = mPosition.y;
	mTransform.m[3][2] = mPosition.z;

	return mTransform;
}

//===================================================================================
void tzCoreTransform::setPosition(const tzVector3D &position)
{
	mPosition = position;
	updateTransformMatrix();
}

//===================================================================================
tzMatrix tzCoreTransform::transformMatrix( ) const
{
	return mTransform;
}

//===================================================================================
tzMatrix tzCoreTransform::invertedTransformMatrix() const
{
	tzMatrix inv;
	if ( !mTransform.invert(inv) )
	{
		printf( "Can't get inverted matrix \n" );
		return tzMatrix();
	}
	return inv;
}

//===================================================================================
tzVector3D tzCoreTransform::position() const
{
	return mPosition;
}

//===================================================================================
tzVector3D  tzCoreTransform::move(float deltaX, float deltaY, float deltaZ)
{
	mPosition += tzVector3D(deltaX, deltaY, deltaZ);
	mTransform.m[3][0] = mPosition.x;
	mTransform.m[3][1] = mPosition.y;
	mTransform.m[3][2] = mPosition.z;
	return mPosition;
}

//===================================================================================
tzVector3D tzCoreTransform::move(tzVector3D offset)
{
	mPosition += offset;
	mTransform.m[3][0] = mPosition.x;
	mTransform.m[3][1] = mPosition.y;
	mTransform.m[3][2] = mPosition.z;
	return mPosition;
}

