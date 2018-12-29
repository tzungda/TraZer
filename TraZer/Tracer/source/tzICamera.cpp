
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
		exposure_time(1.0),
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
		exposure_time(c.exposure_time),
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
	exposure_time 	= rhs.exposure_time;
	mOutputPath		= rhs.mOutputPath;

	return (*this);
}


// ----------------------------------------------------------------- destructor

tzICamera::~tzICamera(void) {}



//-------------------------------------------------------------- compute_uvw

// This computes an orthornormal basis given the view point, lookat point, and up vector

void
tzICamera::compute_uvw(void) {
	w = eye - lookat;
	w.normalize();
	u = up ^ w; 
	u.normalize();
	v = w ^ u;

	// take care of the singularity by hardwiring in specific camera orientations
	
	if (eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) { // camera looking vertically down
		u = Vector3D(0, 0, 1);
		v = Vector3D(1, 0, 0);
		w = Vector3D(0, 1, 0);	
	}
	
	if (eye.x == lookat.x && eye.z == lookat.z && eye.y < lookat.y) { // camera looking vertically up
		u = Vector3D(1, 0, 0);
		v = Vector3D(0, 0, 1);
		w = Vector3D(0, -1, 0);
	}
}

//===================================================================================
void tzICamera::setOutputPath( const std::string &outputPath )
{
	mOutputPath = outputPath;
}

