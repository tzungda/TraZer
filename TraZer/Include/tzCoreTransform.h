#ifndef TZ_TRANSFORM_H
#define TZ_TRANSFORM_H

#include <vector>
#include "tzMatrix.h"
#include "tzVector3D.h"
using namespace std;

/*
The basic class for all the transformation interfaces
*/

class tzCoreTransform
{
public:
	tzCoreTransform(  );
	tzCoreTransform( const tzMatrix &m );
	tzCoreTransform(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
					float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
	virtual ~tzCoreTransform( );

protected:
	float mXAxisAngle;// degree
	float mYAxisAngle;// degree
	float mZAxisAngle;// degree
	tzVector3D mPosition;
	tzMatrix	mTransform;
	tzVector3D	mFace;
	tzVector3D	mUp;
	tzVector3D	mRight;
	tzMatrix mXRotationMatrix;
	tzMatrix mYRotationMatrix;
	tzMatrix mZRotationMatrix;

public:
	tzMatrix		rotateAlongVector(float angle, const tzVector3D &v);
	tzMatrix		roatateX(float deltaAngle);
	tzMatrix		roatateY(float deltaAngle);
	tzMatrix		roatateZ(float deltaAngle);
	tzMatrix		updateTransformMatrix();
	void			setPosition(const tzVector3D& position);
	tzMatrix		transformMatrix( ) const;
	tzMatrix		invertedTransformMatrix() const;
	tzVector3D		position( ) const;
	//
	virtual tzVector3D		move(float deltaX, float deltaY, float deltaZ);
	virtual tzVector3D		move(tzVector3D offset);
};

#endif