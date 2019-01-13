#include "../Include/tzGLCamera.h"
#include "../include/tzConstants.h"
#include "../include/tzCoreCamera.h"

//
#include "tzMatrix.h"
#include "tzNormal.h"

using namespace glm;

//const float degreeToRadian = 3.1415926f / 180.0f;

tzGLCamera::tzGLCamera()
{
	mFace.x = 0.0f, mFace.y = 0.0f, mFace.z = -1.0f;
	mUp.x = 0.0f, mUp.y = 1.0f, mUp.z = 0.0f;
	mRight.x = 1.0f, mRight.y = 0.0f, mRight.z = 0.0f;

	//
	mXAxisAngle = mYAxisAngle = mZAxisAngle = 0.0f;
	//

	SetCamera(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f));
	zoom = 1.0f;
}

mat4 tzGLCamera::GetModelMatrix() 
{
	return mat4();//translationMatrix * rotationMatrix;
}

mat4 tzGLCamera::GetViewMatrix()
{
	//viewMatrix = inverse(mTransform );
	//mat4 m = viewMatrix ;//*translationMatrix * rotationMatrix;
    return mat4();
}

mat4 tzGLCamera::GetProjectionMatrix(float aspect)
{
    mat4 projectionMatrix;
    float nearVal;
    float farVal;
    nearVal = 0.1f;
    farVal = 10000.0f;
    if(ortho) {
        float size = 1.5f * zoom;
        projectionMatrix = glm::ortho(-aspect * size, aspect * size, -size, size, nearVal, farVal);
    } 
	else {
        projectionMatrix = perspective(radians(45.0f * zoom), aspect, nearVal, farVal);	

		
		tzCoreCamera testCam;
		testCam.setAspect( 45.0f ), testCam.setAspect( aspect ), testCam.setNearPlane( nearVal ), testCam.setFarPlane( farVal );

		tzMatrix pers = testCam.perspectiveProjection( );
		int a = 0;
		a = 1;
		

    }
	
    return projectionMatrix;
}

mat4 tzGLCamera::GetViewProjectionMatrix(float aspect)
{
    return GetProjectionMatrix(aspect) * viewMatrix;
}

mat4 tzGLCamera::GetModelViewProjectionMatrix(float aspect)
{
    return GetViewProjectionMatrix(aspect) * GetModelMatrix();
}

vec3 tzGLCamera::GetWorldEyePosition() {
	vec4 wordEyePosition = vec4(eyePosition, 0) * GetModelMatrix();
	return wordEyePosition.xyz;
}

vec3 tzGLCamera::GetWorldViewVector() {
	vec4 wordLookVector = vec4(-viewVector, 0) * GetModelMatrix();
	return wordLookVector.xyz;
}

void tzGLCamera::keyEvents(unsigned char key) {
	switch (key)
	{
	case 'w': 
	case 'W':
		Translate(vec3(0, 0, moveSpeed));
		break;
	case 'a':
	case 'A':
		Translate(vec3(moveSpeed, 0, 0));
		break;
	case 's':
	case 'S':
		Translate(vec3(0, 0, -moveSpeed));
		break;
	case 'd':
	case 'D':
		Translate(vec3(-moveSpeed, 0, 0));
		break; 
	case 'q':
	case 'Q':
		Translate(vec3(0, -moveSpeed, 0));
		break;
	case 'e':
	case 'E':
		Translate(vec3(0, moveSpeed, 0));
		break;
	case '+':
		wheelEvent(-moveSpeed);
		break;
	case '-':
		wheelEvent(moveSpeed);
		break;
	default:
		break;
	}
}

void tzGLCamera::mouseEvents(int button, int state, int x, int y) {

	if (state == GLUT_UP)
	{
		mouseReleaseEvent(button, x, y);
	}
	else if (state == GLUT_DOWN) 
	{
		mousePressEvent(button, x, y);
	}
	if (button == 4) 
	{
		wheelEvent(1);
	}
	else if (button == 3)
	{
		wheelEvent(-1);
	}
}

void tzGLCamera::mousePressEvent(int button, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON)
	{
        lmbDown = true;
        lmbDownCoord = vec2(x,y);
		
        //mat4 invrtRot = inverse(rotationMatrix);
        //rotateYAxis = (invrtRot * vec4(0, 1, 0, 0)).xyz;
        //rotateXAxis = (invrtRot * /*vec4(1, 0, 0, 0)*/vec4(rightDir(), 0.0f)).xyz;
		
    } 
	else if(button == GLUT_MIDDLE_BUTTON)
	{
        midDown = true;
        midDownCoord = vec2(x, y);
    }
	else if ( button == GLUT_RIGHT_BUTTON )
	{
		rmbDown = true;
		rmbDownCoord = vec2(x, y);
		//mat4 invrtRot = inverse(rotationMatrix);
		//rotateZAxis = (invrtRot * /*vec4(1, 0, 0, 0)*/vec4(faceDir(), 0.0f)).xyz;
	}
}

void tzGLCamera::mouseReleaseEvent(int button, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON) // rotation
	{
        lmbDown = false;
    }
	else if(button == GLUT_MIDDLE_BUTTON || button == 3 || button == 4) { // pan
        midDown = false;
    }
	else if(button == GLUT_RIGHT_BUTTON) // zoomn
	{
		rmbDown = false;
	}
}

void tzGLCamera::mouseMoveEvent(int x,int y)
{
    if(lmbDown)
	{
        vec2 coord = vec2(x, y);
		vec2 diff = coord - lmbDownCoord;
        float factor = 0.06f;
		tzMatrix rotM;
		if ( fabs( diff.x ) > fabs( diff.y ) )
		{
			rotM = rotateAlongVector(diff.x*factor, mUp );//upDir());
		}
		else
		{
			rotM = rotateAlongVector(diff.y*factor, rightDir());
		}
		float angleX = atan2(rotM.m[1][2], rotM.m[2][2]) / degreeToRadian;
		float angleY = atan2(-rotM.m[0][2], sqrtf(rotM.m[1][2] * rotM.m[1][2] + rotM.m[2][2] * rotM.m[2][2])) / degreeToRadian;
		float angleZ = atan2(rotM.m[0][1], rotM.m[0][0]) / degreeToRadian;
		roatateX( angleX );
		roatateY( angleY );
		roatateZ( angleZ );
		updateTransformMatrix();
			//glm::vec3 up = mUp;//upDir();
			//rotationMatrix = rotate( rotationMatrix ,diff.x * factor, /* rotateYAxis*/ upDir() );
			//vec3 dir = normalize(viewVector*mat3(rotationMatrix) );
			//float len = ( eyeLookPosition - eyePosition ).length();
			//viewMatrix = lookAt( eyePosition, eyePosition - dir*len, upDir() );

			//mRight = mRight*glm::mat3(rotationMatrix), mRight = normalize(mRight );
			//mFace = mFace*glm::mat3(rotationMatrix), mFace = normalize(mFace);
		
		
        lmbDownCoord = coord;
    }
	else if(midDown)
	{
		vec2 coord = vec2(x,y);
		vec2 diff = coord - midDownCoord;

		mPosition += upDir() * diff.y * 10.0f / (float)w_height;
		mPosition -= rightDir() * diff.x * 10.0f / (float)w_height;
		updateTransformMatrix();

		//vec4 up( upDir(), 0.0f ); //vec4(0, 1, 0, 0);
		//vec4 right( rightDir(), 0.0f ); //vec4(1, 0, 0, 0);

		//vec3 diffUp = up.xyz * diff.y / (float)w_height;
		//vec3 diffRight = right.xyz * diff.x / (float)w_width;

		//eyeLookPosition += ( diffUp - diffRight )*3.0f;
		//eyePosition += (diffUp - diffRight)*3.0f;

		//viewMatrix = lookAt(eyePosition, eyeLookPosition, vec3( 0.0f, 1.0f, 0.0f ) );
        //translationMatrix = translate(translationMatrix, (-diffUp + diffRight) * zoom * 3.0f);
        midDownCoord = coord;
    }
	else if( rmbDown )
	{
		vec2 coord = vec2(x, y);
		vec2 diff = coord - rmbDownCoord;
		rmbDownCoord = coord;
		float d = 0.02f;
		if (fabs(diff.x) > fabs(diff.y))
		{
			d *= diff.x;
		}
		else
		{
			d *= diff.y;
		}
		mPosition += faceDir()*d*3.0f;
		updateTransformMatrix();
		//translationMatrix = translate(translationMatrix, rotateZAxis * d * 3.0f);
		//eyeLookPosition += faceDir()*d*3.0f;
		//eyePosition += faceDir()*d*3.0f;

		//viewMatrix = lookAt(eyePosition, eyeLookPosition, vec3(0.0f, 1.0f, 0.0f));
	}
}

void tzGLCamera::wheelEvent(float direction)
{
	wheel_val = direction * 15.0f;
    Zoom(wheel_val / 120.0f);
}

void tzGLCamera::Zoom(float distance)
{
    zoom *= (1.0f + 0.05f * distance);
    zoom = clamp(0.1f, zoom, 3.0f);
}

void tzGLCamera::SetCamera(glm::vec3 cameraPos, glm::vec3 focusPos) {
	eyePosition = cameraPos;
	eyeLookPosition = focusPos;

	//
	//mFace = normalize( focusPos - cameraPos );
	//mRight = normalize( cross(mFace, mUp) );
	//mUp = normalize(cross(mRight, mFace));
	//
	viewVector = eyePosition - eyeLookPosition;
	viewVector = normalize(viewVector);

	vec3 up = vec3(0, 1, 0);
	viewMatrix = lookAt(eyePosition, eyePosition - viewVector, up);

	/*
	tmat4x4(
	T const & x0, T const & y0, T const & z0, T const & w0,
	T const & x1, T const & y1, T const & z1, T const & w1,
	T const & x2, T const & y2, T const & z2, T const & w2,
	T const & x3, T const & y3, T const & z3, T const & w3);
	*/
	//setPosition(tzVector3D(50.0f, 70.0f, 50.0f));
	setPosition(tzVector3D(0.0f, 0.0f, 60.0f));
	mXAxisAngle = mYAxisAngle = mZAxisAngle = 0.0f;
/*	tzMatrix rotXMatrixX = roatateX( -45.0f );
	tzMatrix r0 = rotateAlongVector( -45.0f, tzVector3D( 1.0f, 0.0f, 0.0f ) );
	tzMatrix rotYXMatrix = roatateY(45.0f);
	tzMatrix r1 = rotateAlongVector(45.0f, tzVector3D(0.0f, 1.0f, 0.0f));*/
	updateTransformMatrix();

	float angleX = atan2( mTransform.m[1][2], mTransform.m[2][2] )/degreeToRadian;
	float angleY = atan2( -mTransform.m[0][2], sqrt(mTransform.m[1][2]* mTransform.m[1][2] + mTransform.m[2][2]* mTransform.m[2][2]) ) / degreeToRadian;
	float angleZ = atan2( mTransform.m[0][1], mTransform.m[0][0] ) / degreeToRadian;
	/*
	const float ff = 3.1415926f/180.0f;
	mat4 viewMatrixTest(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, cos(-45.0f*ff), sin(-45.0f*ff), 0.0f,
							0.0f, -sin(-45.0f*ff), cos(-45.0f*ff), 0.0f,
							0.0f, 50.0f, 50.0f, 1.0f );
	*/
	
	//viewMatrix = inverse(mTransform);

	// test------------------------------------------------------------
	//vec4 test = vec4( 0, 1, 0, 0 );
	//test = test*viewMatrix;
	// test------------------------------------------------------------

	//viewVector = eyePosition - eyeLookPosition;
	viewVector = normalize(viewVector);
}

void tzGLCamera::SetWindowSize(int width,int height) {
	w_width = width;
	w_height = height;
}

/*
void tzGLCamera::SetRotation(float theta, float phi)
{
    rotationMatrix = mat4(1.0);
    rotationMatrix = rotate(rotationMatrix, theta, vec3(0, 1, 0));
    rotationMatrix = rotate(rotationMatrix, phi, vec3(1, 0, 0));
}

void tzGLCamera::SetRotation(float x, float y, float z)
{
	vec3 v(x, y, z);
    v = normalize(v);
	vec3 o(0, 0, 1);
    double angle = acos(dot(v, o));
    rotationMatrix = mat4(1.0);
    rotationMatrix = rotate(rotationMatrix, (float)angle, cross(o, v));
}
*/

void tzGLCamera::Reset()
{
	wheel_val = 0.0f;
    zoom = 1.0f;
    translationMatrix = mat4(1.0);
    rotationMatrix = mat4(1.0);
}

void tzGLCamera::Translate(vec3 vec) {
	vec3 diff = vec;

	vec4 up = vec4(0, 1, 0, 0);
	vec4 right = vec4(1, 0, 0, 0);
	vec4 front = vec4(0, 0, 1, 0);

	vec3 diffUp = up.xyz * diff.y;
	vec3 diffRight = right.xyz * diff.x;
	vec3 diffFront = front.xyz * diff.z;

	translationMatrix = translate(translationMatrix, (-diffUp + diffRight + diffFront) * zoom * 3.0f);
}

//===================================================================================
tzVector3D tzGLCamera::faceDir() const
{
	//glm::vec3 face = glm::mat3(mZRotationMatrix*mYRotationMatrix*mXRotationMatrix)*mFace;

	tzVector3D face = mFace*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	face.normalize();

	return face;
}

//===================================================================================
tzVector3D tzGLCamera::upDir() const
{
	//glm::vec3 up = glm::mat3(mZRotationMatrix*mYRotationMatrix*mXRotationMatrix)*mUp;

	tzVector3D up = mUp*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	up.normalize();

	return up;
}

//===================================================================================
tzVector3D tzGLCamera::rightDir() const
{
	//glm::vec3 right = glm::mat3(mZRotationMatrix*mYRotationMatrix*mXRotationMatrix)*mRight;

	tzVector3D right = mRight*mXRotationMatrix*mYRotationMatrix*mZRotationMatrix;
	right.normalize( );

	return right;
}


//===================================================================================
tzVector3D  tzGLCamera::move(float deltaX, float deltaY, float deltaZ)
{
	tzVector3D up = upDir( );
	tzVector3D face = faceDir( );
	tzVector3D right = rightDir( );
	mPosition += right*deltaX + up*deltaY + face*deltaZ ;

	return mPosition;
}


