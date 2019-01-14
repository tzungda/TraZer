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



#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>

#include "../Include/tzCoreTransform.h"
#include "../include/tzCoreCamera.h"
#include "tzPoint2D.h"
//#include "tzMatrix.h"
//#include "tzVector3D.h"

class tzNormal;


class tzGLCamera
{
public:
    tzGLCamera();

	void mouseEvents(int button, int state, int x, int y);
	void mousePressEvent(int button, int x, int y);
	void mouseReleaseEvent(int button, int x, int y);
    void mouseMoveEvent(int x,int y);
	void keyEvents(unsigned char key);
    void wheelEvent(float direction);

	tzMatrix viewMatrix();
	tzMatrix projectionMatrix(float aspect);


    bool IsOrthoProjection() { return mIsOrtho; }

	void setWindowSize(int width, int height);

    bool ToggleOrtho() { return mIsOrtho = !mIsOrtho; }
    void zoom(float distance);
    void reset();

	void		setCamPosition(const tzVector3D& position);

	

private:
    bool mIsOrtho;// = false;
    float mZoom;// = 1.0f;
	float mMoveSpeed;// = 50.0f;

	tzCoreCamera mCamera;
	//----------------------------------

    bool mLmbDown = false;
    bool mMidDown = false;
	bool mRmbDown = false;
	tzPoint2D mLmbDownCoord;
	tzPoint2D mMidDownCoord;
	tzPoint2D mRmbDownCoord;
	
	int mWindowWidth;
	int mWindowHeight;
	float mWheelVal;
};


#endif // tzGLCamera_H