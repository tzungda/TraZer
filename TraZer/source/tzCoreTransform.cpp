#include "../Include/tzCoreTransform.h"
#include "../Include/tzConstants.h"

//===================================================================================
tzCoreTransform::tzCoreTransform(): mScale(tzVector3D(1.0f, 1.0f, 1.0f ) )
{
	mXAxisAngle = mYAxisAngle = mZAxisAngle = 0.0f;
	mTransform.setIdentity();
	mInitMatrix.setIdentity();
}

//===================================================================================
tzCoreTransform::tzCoreTransform(const tzMatrix &m)
{
	tzCoreTransform();
	mInitMatrix = m;
}

//===================================================================================
tzCoreTransform::tzCoreTransform(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	tzCoreTransform();
	mInitMatrix = tzMatrix(	m00, m01, m02, m03, m10, m11, m12, m13,
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
tzMatrix  tzCoreTransform::rotateX(float deltaAngle)
{
	mXAxisAngle += deltaAngle;
	const float rad = mXAxisAngle * degreeToRadian;

	mXRotationMatrix = tzMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(rad), sin(rad), 0.0f,
		0.0f, -sin(rad), cos(rad), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	updateTransformMatrix();
	return mXRotationMatrix;
}

//===================================================================================
tzMatrix  tzCoreTransform::rotateY(float deltaAngle)
{
	mYAxisAngle += deltaAngle;
	const float rad = mYAxisAngle*degreeToRadian;

	mYRotationMatrix = tzMatrix(cos(rad), 0.0f, -sin(rad), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(rad), 0.0f, cos(rad), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	updateTransformMatrix();
	return mYRotationMatrix;
}

//===================================================================================
tzMatrix  tzCoreTransform::rotateZ(float deltaAngle)
{
	mZAxisAngle += deltaAngle;
	const float rad = mZAxisAngle * degreeToRadian;
 
	mZRotationMatrix = tzMatrix(cos(rad), sin(rad), 0.0f, 0.0f,
		-sin(rad), cos(rad), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	updateTransformMatrix();
	return mZRotationMatrix;
}

//===================================================================================
tzMatrix  tzCoreTransform::updateTransformMatrix()
{
	mTransform.setIdentity();
	mTransform.m[0][0] = mScale.x;
	mTransform.m[1][1] = mScale.y;
	mTransform.m[2][2] = mScale.z;
	mTransform = mTransform*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	
	
	mTransform.m[3][0] = mPosition.x;
	mTransform.m[3][1] = mPosition.y;
	mTransform.m[3][2] = mPosition.z;

	mTransform = mTransform*mInitMatrix;

	return mTransform;
}

//===================================================================================
void tzCoreTransform::setInitMatrix(const tzMatrix& mat)
{
	mInitMatrix = mat;
}

//===================================================================================
void tzCoreTransform::setPosition(const tzVector3D &position)
{
	mPosition = position;
	updateTransformMatrix();
}

//===================================================================================
void tzCoreTransform::setScale( const tzVector3D& scale )
{
	mScale = scale;
	updateTransformMatrix();
}

//===================================================================================
tzMatrix tzCoreTransform::transformMatrix( ) const
{
	//tzCoreTransform temp = *this;
	//temp.updateTransformMatrix();
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

