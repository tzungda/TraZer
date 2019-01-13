
#include "../include/tzICamera.h"


// ----------------------------------------------------------------- default constructor

tzICamera::tzICamera(void)
	:	eye(0, 0, 500),
		lookat(0),
		ra(0),
		up(0, 1, 0),
		u(1, 0, 0),
		v(0, 1, 0),
		w(0, 0, 1),
		mExposureTime(1.0),
		mOutputPath( "C:\\Temp\\test.png" )
{}


// ----------------------------------------------------------------- copy constructor

tzICamera::tzICamera(const tzICamera& c)
	: 	eye(c.eye),
		lookat(c.lookat),
		ra(c.ra),
		up(c.up),
		u(c.u),
		v(c.v),
		w(c.w),
		mExposureTime(c.mExposureTime),
		mOutputPath(c.mOutputPath)
{}



// ----------------------------------------------------------------- assignment operator

tzICamera&
tzICamera::operator= (const tzICamera& rhs) {
	if (this == &rhs)
		return (*this);
	
	eye				= rhs.eye;
	lookat			= rhs.lookat;
	ra				= rhs.ra;
	up				= rhs.up;
	u				= rhs.u;
	v				= rhs.v;
	w				= rhs.w;
	mExposureTime = rhs.mExposureTime;
	mOutputPath		= rhs.mOutputPath;

	return (*this);
}


// ----------------------------------------------------------------- destructor

tzICamera::~tzICamera(void) {}



//-------------------------------------------------------------- computeUVW

// This computes an orthornormal basis given the view point, lookat point, and up vector

void
tzICamera::computeUVW(void) {
	w = eye - lookat;
	w.normalize();
	u = up ^ w; 
	u.normalize();
	v = w ^ u;

	// take care of the singularity by hardwiring in specific camera orientations
	
	if (eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) { // camera looking vertically down
		u = tzVector3D(0, 0, 1);
		v = tzVector3D(1, 0, 0);
		w = tzVector3D(0, 1, 0);
	}
	
	if (eye.x == lookat.x && eye.z == lookat.z && eye.y < lookat.y) { // camera looking vertically up
		u = tzVector3D(1, 0, 0);
		v = tzVector3D(0, 0, 1);
		w = tzVector3D(0, -1, 0);
	}
}

//===================================================================================
void tzICamera::setOutputPath( const std::string &outputPath )
{
	mOutputPath = outputPath;
}

