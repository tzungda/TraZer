
#include "../include/tzICamera.h"


// ----------------------------------------------------------------- default constructor

tzICamera::tzICamera(void)
	:	mEye(0, 0, 500),
		mLookAt(0),
		mRollAngle(0),
		mUp(0, 1, 0),
		mOrthoU(1, 0, 0),
		mOrthoV(0, 1, 0),
		mOrthoW(0, 0, 1),
		mExposureTime(1.0),
		mOutputPath( "C:\\Temp\\test.png" )
{}


// ----------------------------------------------------------------- copy constructor

tzICamera::tzICamera(const tzICamera& c)
	: 	mEye(c.mEye),
		mLookAt(c.mLookAt),
		mRollAngle(c.mRollAngle),
		mUp(c.mUp),
		mOrthoU(c.mOrthoU),
		mOrthoV(c.mOrthoV),
		mOrthoW(c.mOrthoW),
		mExposureTime(c.mExposureTime),
		mOutputPath(c.mOutputPath)
{}



// ----------------------------------------------------------------- assignment operator

tzICamera&
tzICamera::operator= (const tzICamera& rhs) {
	if (this == &rhs)
		return (*this);
	
	mEye				= rhs.mEye;
	mLookAt			= rhs.mLookAt;
	mRollAngle		= rhs.mRollAngle;
	mUp				= rhs.mUp;
	mOrthoU				= rhs.mOrthoU;
	mOrthoV				= rhs.mOrthoV;
	mOrthoW				= rhs.mOrthoW;
	mExposureTime = rhs.mExposureTime;
	mOutputPath		= rhs.mOutputPath;

	return (*this);
}


// ----------------------------------------------------------------- destructor

tzICamera::~tzICamera(void) {}



//-------------------------------------------------------------- computeUVW

// This computes an orthornormal basis given the view point, lookat point, and up vector

void
tzICamera::computeUVW(void) 
{
	mOrthoW = mEye - mLookAt;
	mOrthoW.normalize();
	mOrthoU = mUp ^ mOrthoW;
	mOrthoU.normalize();
	mOrthoV = mOrthoW ^ mOrthoU;

	// take care of the singularity by hardwiring in specific camera orientations
	
	if (mEye.x == mLookAt.x && mEye.z == mLookAt.z && mEye.y > mLookAt.y) // camera looking vertically down
	{ 
		mOrthoU = tzVector3D(0, 0, 1);
		mOrthoV = tzVector3D(1, 0, 0);
		mOrthoW = tzVector3D(0, 1, 0);
	}
	
	if (mEye.x == mLookAt.x && mEye.z == mLookAt.z && mEye.y < mLookAt.y) // camera looking vertically up
	{ 
		mOrthoU = tzVector3D(1, 0, 0);
		mOrthoV = tzVector3D(0, 0, 1);
		mOrthoW = tzVector3D(0, -1, 0);
	}
}

//===================================================================================
void tzICamera::setOutputPath( const std::string &outputPath )
{
	mOutputPath = outputPath;
}

