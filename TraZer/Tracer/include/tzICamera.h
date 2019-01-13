#ifndef TZ_TRACER_ICAMERA
#define TZ_TRACER_ICAMERA


#include "tzPoint3D.h"
#include "tzVector3D.h"
#include <string>

class tzWorld;  // can't #include "World" here because World contains a camera pointer

//--------------------------------------------------------------------- class Camera

class tzICamera 
{
	public:
	
		tzICamera();   							// default constructor

		tzICamera(const tzICamera& tzICamera);			// copy constructor
		
		virtual tzICamera* clone(void) const = 0;
		
		virtual ~tzICamera();   							

		virtual void renderScene(const tzWorld& w) const = 0;
		
		void setEye(const tzPoint3D& p);

		void setEye(const float x, const float y, const float z);
		
		void setLookAt(const tzPoint3D& p);

		void setLookAt(const float x, const float y, const float z);

		void setUpVector(const tzVector3D& u);

		void setUpVector(const float x, const float y, const float z);

		void setRoll(const float ra);
		
		void setExposureTime(const float exposure);
		
		void computeUVW(void);

		void setOutputPath( const std::string &outputPath );
		
		
	protected:		
	
		tzPoint3D		eye;				// eye point
		tzPoint3D		lookat; 			// lookat point
		float			ra;					// roll angle
		tzVector3D		u, v, w;			// orthonormal basis vectors
		tzVector3D		up;					// up vector
		float			mExposureTime;

		std::string		mOutputPath;
		
		tzICamera& 							// assignment operator
		operator= (const tzICamera& camera);
};


// inlined access functions


// ----------------------------------------------------------------- setEye

inline void
tzICamera::setEye(const tzPoint3D& p) {
	eye = p;
}


// ----------------------------------------------------------------- setEye

inline void
tzICamera::setEye(const float x, const float y, const float z) {
	eye.x = x; eye.y = y; eye.z = z;
}


// ----------------------------------------------------------------- setLookAt

inline void
tzICamera::setLookAt(const tzPoint3D& p) {
	lookat = p;
}


// ----------------------------------------------------------------- setLookAt

inline void
tzICamera::setLookAt(const float x, const float y, const float z) {
	lookat.x = x; lookat.y = y; lookat.z = z;
}


// ----------------------------------------------------------------- setUpVector

inline void
tzICamera::setUpVector(const tzVector3D& u) {
	up = u;
}


// ----------------------------------------------------------------- setUpVector

inline void
tzICamera::setUpVector(const float x, const float y, const float z) {
	up.x = x; up.y = y; up.z = z;
}


// ----------------------------------------------------------------- setRoll

inline void
tzICamera::setRoll(const float r) {
	ra = r;
}


// ----------------------------------------------------------------- set_exposure_time

inline void
tzICamera::setExposureTime(const float exposure) 
{
	mExposureTime = exposure;
}


#endif
