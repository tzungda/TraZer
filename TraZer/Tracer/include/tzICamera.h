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
		
		virtual tzICamera*							// virtual copy constructor
		clone(void) const = 0;
		
		virtual
		~tzICamera();   							

		virtual void renderScene(const tzWorld& w) const = 0;
		
		void
		set_eye(const tzPoint3D& p);

		void
		set_eye(const float x, const float y, const float z);
		
		void
		set_lookat(const tzPoint3D& p);

		void
		set_lookat(const float x, const float y, const float z);

		void
		set_up_vector(const tzVector3D& u);

		void
		set_up_vector(const float x, const float y, const float z);

		void
		set_roll(const float ra);
		
		void setExposureTime(const float exposure);
		
		void									
		compute_uvw(void);

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


// ----------------------------------------------------------------- set_eye

inline void
tzICamera::set_eye(const tzPoint3D& p) {
	eye = p;
}


// ----------------------------------------------------------------- set_eye

inline void
tzICamera::set_eye(const float x, const float y, const float z) {
	eye.x = x; eye.y = y; eye.z = z;
}


// ----------------------------------------------------------------- set_lookat

inline void
tzICamera::set_lookat(const tzPoint3D& p) {
	lookat = p;
}


// ----------------------------------------------------------------- set_lookat

inline void
tzICamera::set_lookat(const float x, const float y, const float z) {
	lookat.x = x; lookat.y = y; lookat.z = z;
}


// ----------------------------------------------------------------- set_up_vector

inline void
tzICamera::set_up_vector(const tzVector3D& u) {
	up = u;
}


// ----------------------------------------------------------------- set_up_vector

inline void
tzICamera::set_up_vector(const float x, const float y, const float z) {
	up.x = x; up.y = y; up.z = z;
}


// ----------------------------------------------------------------- set_roll

inline void
tzICamera::set_roll(const float r) {
	ra = r;
}


// ----------------------------------------------------------------- set_exposure_time

inline void
tzICamera::setExposureTime(const float exposure) 
{
	mExposureTime = exposure;
}


#endif
