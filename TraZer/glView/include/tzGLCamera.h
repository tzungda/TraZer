#ifndef TZ_CAMERA_H
#define TZ_CAMERA_H

#ifdef _MSC_VER
	#include "GLEW/glew.h"
	#include "FreeGLUT/freeglut.h"
	#include <direct.h>
#else
	#include <OpenGL/gl3.h>
	#include <GLUT/glut.h>
	#include <unistd.h>
#endif

#define GLM_SWIZZLE
#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"
#include "GLM/glm/gtx/rotate_vector.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>

#include "../Include/tzCoreTransform.h"
//#include "tzMatrix.h"
//#include "tzVector3D.h"

class tzNormal;

/**
 * @brief The tzGLCamera class
 * The tzGLCamera class provides viewing manipulation related functionalities.
 *
 * To use the tzGLCamera class, call mousePressEvent(),
 * mouseReleaseEvent(), mouseMoveEvent(), wheelEvent() in your event handlers
 * with the event objects.
 *
 * The viewing manipulation will be done for you in the class. When you are ready
 * to render something, call GetModelMatrix(), GetViewMatrix(), GetProjectionMatrix()
 * and their composite versions to get the MVP matrices which ecode current viewing
 * properties.
 */

class tzGLCamera: public tzCoreTransform
{
public:
    tzGLCamera();

	void mouseEvents(int button, int state, int x, int y);
	void mousePressEvent(int button, int x, int y);
	void mouseReleaseEvent(int button, int x, int y);
    void mouseMoveEvent(int x,int y);
	void keyEvents(unsigned char key);
    void wheelEvent(float direction);
	void Translate(glm::vec3 vec);

	glm::mat4 GetModelMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float aspect);
	glm::mat4 GetViewProjectionMatrix(float aspect);
	glm::mat4 GetModelViewProjectionMatrix(float aspect);
	glm::vec3 GetEyePosition() {return eyePosition;}
	glm::vec3 GetViewVector() {return viewVector;}

	glm::vec3 GetWorldEyePosition();
	glm::vec3 GetWorldViewVector();

    bool IsOrthoProjection() { return ortho; }

	void SetCamera(glm::vec3 cameraPos, glm::vec3 focusPos);
//    void SetRotation(float theta, float phi);
//    void SetRotation(float x, float y, float z);
	void SetWindowSize(int width, int height);

    bool ToggleOrtho() { return ortho = !ortho; }
    void Zoom(float distance);
    void Reset();

	//
	tzVector3D	faceDir( ) const;
	tzVector3D	upDir() const;
	tzVector3D	rightDir() const;

	//
	/*
	tzMatrix	rotateMatrixAlongVector( float angle, const tzVector3D &v );
	tzMatrix	roatateX( float deltaAngle );
	tzMatrix	roatateY( float deltaAngle );
	tzMatrix	roatateZ( float deltaAngle );
	tzMatrix	updateTransformMatrix( );
	void		setPosition( const tzVector3D& position );
	*/
	virtual tzVector3D	move( float deltaX, float deltaY, float deltaZ );
	//
	//tzMatrix invertedTransformMatrix( ) const;
	

private:
    bool ortho = false;
    float zoom = 1.0f;
	float moveSpeed = 50.0f;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 viewVector;
	glm::vec3 rotateXAxis;
	glm::vec3 rotateYAxis;
	glm::vec3 rotateZAxis;
	glm::vec3 eyePosition;
	glm::vec3 eyeLookPosition;

	//----------------------------------
	//glm::mat4 mTransform;
	//glm::mat4 mXRotationMatrix;
	//glm::mat4 mYRotationMatrix;
	//glm::mat4 mZRotationMatrix;
	//float mXAxisAngle;// degree
	//float mYAxisAngle;// degree
	//float mZAxisAngle;// degree
	//tzVector3D mPosition;
	//glm::vec3	mFace;
	//glm::vec3	mUp;
	//glm::vec3	mRight;
	// mine
	//tzMatrix	mTransform;
	tzVector3D	mFace;
	tzVector3D	mUp;
	tzVector3D	mRight;
	//tzMatrix mXRotationMatrix;
	//tzMatrix mYRotationMatrix;
	//tzMatrix mZRotationMatrix;
	//----------------------------------

    bool lmbDown = false;
    bool midDown = false;
	bool rmbDown = false;
	glm::vec2 lmbDownCoord;
	glm::vec2 midDownCoord;
	glm::vec2 rmbDownCoord;
	
	int w_width;
	int w_height;
	float wheel_val;
};


#endif // tzGLCamera_H