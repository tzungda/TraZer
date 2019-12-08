

#include "../Include/tzGLCamera.h"
#include "../include/tzConstants.h"


//
#include "tzMatrix.h"
#include "tzNormal.h"
#include "tzMaths.h"


//===================================================================================
tzGLCamera::tzGLCamera()
{
	mIsOrtho = false;
	mMoveSpeed = 50.0f;
	mZoom = 1.0f;
}

//===================================================================================
tzMatrix tzGLCamera::viewMatrix()
{
    return mCamera.invertedTransformMatrix();
}

//===================================================================================
tzMatrix tzGLCamera::projectionMatrix(float aspect)
{
	return mCamera.perspectiveProjection();
	//return mCamera.orthogonalProjection();

	
	/*
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

		
		//tzCoreCamera testCam;
		mCamera.setAspect( 45.0f ), mCamera.setAspect( aspect ), mCamera.setNearPlane( nearVal ), mCamera.setFarPlane( farVal );

		//tzMatrix pers = testCam.perspectiveProjection( );
		//int a = 0;
		//a = 1;
		

    }
	
	return mCamera.perspectiveProjection();
    //return projectionMatrix;
	*/
}

//===================================================================================
void tzGLCamera::keyEvents(unsigned char key) {
	switch (key)
	{
	case 'w': 
	case 'W':
		//Translate(vec3(0, 0, moveSpeed));
		break;
	case 'a':
	case 'A':
		//Translate(vec3(moveSpeed, 0, 0));
		break;
	case 's':
	case 'S':
		//Translate(vec3(0, 0, -moveSpeed));
		break;
	case 'd':
	case 'D':
		//Translate(vec3(-moveSpeed, 0, 0));
		break; 
	case 'q':
	case 'Q':
		//Translate(vec3(0, -moveSpeed, 0));
		break;
	case 'e':
	case 'E':
		//Translate(vec3(0, moveSpeed, 0));
		break;
	case '+':
		wheelEvent(-mMoveSpeed);
		break;
	case '-':
		wheelEvent(mMoveSpeed);
		break;
	default:
		break;
	}
}

//===================================================================================
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

//===================================================================================
void tzGLCamera::mousePressEvent(int button, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON)
	{
        mLmbDown = true;
        mLmbDownCoord = tzPoint2D((float)x, (float)y);
		
    } 
	else if(button == GLUT_MIDDLE_BUTTON)
	{
        mMidDown = true;
        mMidDownCoord = tzPoint2D((float)x, (float)y);
    }
	else if ( button == GLUT_RIGHT_BUTTON )
	{
		mRmbDown = true;
		mRmbDownCoord = tzPoint2D((float)x, (float)y);
	}
}

//===================================================================================
void tzGLCamera::mouseReleaseEvent(int button, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON) // rotation
	{
        mLmbDown = false;
    }
	else if(button == GLUT_MIDDLE_BUTTON || button == 3 || button == 4) { // pan
        mMidDown = false;
    }
	else if(button == GLUT_RIGHT_BUTTON) // zoomn
	{
		mRmbDown = false;
	}
}

//===================================================================================
void tzGLCamera::mouseMoveEvent(int x,int y)
{
    if(mLmbDown)
	{
        tzPoint2D coord((float)x, (float)y);
		tzPoint2D diff = coord - mLmbDownCoord;
        float factor = 0.06f;
		tzMatrix rotM;
		if ( fabs( diff.x ) > fabs( diff.y ) )
		{
			rotM = mCamera.rotateAlongVector(diff.x*factor, mCamera.upDir());//upDir());
		}
		else
		{
			rotM = mCamera.rotateAlongVector(diff.y*factor, mCamera.rightDir());
		}
		float angleX = atan2(rotM.m[1][2], rotM.m[2][2]) / degreeToRadian;
		float angleY = atan2(-rotM.m[0][2], sqrtf(rotM.m[1][2] * rotM.m[1][2] + rotM.m[2][2] * rotM.m[2][2])) / degreeToRadian;
		float angleZ = atan2(rotM.m[0][1], rotM.m[0][0]) / degreeToRadian;
		mCamera.roatateX( angleX );
		mCamera.roatateY( angleY );
		mCamera.roatateZ( angleZ );
		mCamera.updateTransformMatrix();

        mLmbDownCoord = coord;
    }
	else if(mMidDown)
	{
		tzPoint2D coord((float)x, (float)y);
		tzPoint2D diff = coord - mMidDownCoord;

		mCamera.move(mCamera.upDir() * diff.y * 10.0f / (float)mWindowHeight);
		mCamera.move(mCamera.rightDir() * diff.x * -10.0f / (float)mWindowWidth);

        mMidDownCoord = coord;
    }
	else if( mRmbDown )
	{
		tzPoint2D coord((float)x, (float)y);
		tzPoint2D diff = coord - mRmbDownCoord;
		mRmbDownCoord = coord;
		float d = 0.02f;
		if (fabs(diff.x) > fabs(diff.y))
		{
			d *= diff.x;
		}
		else
		{
			d *= diff.y;
		}
		mCamera.move(mCamera.faceDir()*d*3.0f);

		mRmbDownCoord = coord;
	}
}

//===================================================================================
void tzGLCamera::wheelEvent(float direction)
{
	mWheelVal = direction * 15.0f;
    zoom(mWheelVal / 120.0f);
}

//===================================================================================
void tzGLCamera::zoom(float distance)
{
    mZoom *= (1.0f + 0.05f * distance);
    mZoom = clamp(0.1f, mZoom, 3.0f);
}

//===================================================================================
void tzGLCamera::setWindowSize(int width,int height) 
{
	mWindowWidth = width;
	mWindowHeight = height;
}

//===================================================================================
void tzGLCamera::reset()
{
	mWheelVal = 0.0f;
    mZoom = 1.0f;
	mCamera = tzCoreCamera();
}

//===================================================================================
void tzGLCamera::setCamPosition(const tzVector3D& position)
{
	mCamera.setPosition( position );
}

