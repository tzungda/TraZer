#ifndef TZ_TRACER_ICAMERA
#define TZ_TRACER_ICAMERA


#include "Point3D.h"
#include "Vector3D.h"
#include <string>

class tzWorld;  // can't #include "World" here because World contains a camera pointer

//--------------------------------------------------------------------- class Camera

class tzICamera {
	public:
	
		tzICamera();   							// default constructor

		tzICamera(const tzICamera& tzICamera);			// copy constructor
		
		virtual tzICamera*							// virtual copy constructor
		clone(void) const = 0;
		
		virtual
		~tzICamera();   							

		virtual void 																		
		render_scene(const tzWorld& w) = 0;
		
		void
		set_eye(const Point3D& p);

		void
		set_eye(const float x, const float y, const float z);
		
		void
		set_lookat(const Point3D& p);

		void
		set_lookat(const float x, const float y, const float z);

		void
		set_up_vector(const Vector3D& u);

		void
		set_up_vector(const float x, const float y, const float z);

		void
		set_roll(const float ra);
		
		void
		set_exposure_time(const float exposure);
		
		void									
		compute_uvw(void);

		void setOutputPath( const std::string &outputPath );
		
		
	protected:		
	
		Point3D			eye;				// eye point
		Point3D			lookat; 			// lookat point
		float			ra;					// roll angle
		Vector3D		u, v, w;			// orthonormal basis vectors
		Vector3D		up;					// up vector
		float			exposure_time;

		std::string		mOutputPath;
		
		tzICamera& 							// assignment operator
		operator= (const tzICamera& camera);
};


// inlined access functions


// ----------------------------------------------------------------- set_eye

inline void
tzICamera::set_eye(const Point3D& p) {
	eye = p;
}


// ----------------------------------------------------------------- set_eye

inline void
tzICamera::set_eye(const float x, const float y, const float z) {
	eye.x = x; eye.y = y; eye.z = z;
}


// ----------------------------------------------------------------- set_lookat

inline void
tzICamera::set_lookat(const Point3D& p) {
	lookat = p;
}


// ----------------------------------------------------------------- set_lookat

inline void
tzICamera::set_lookat(const float x, const float y, const float z) {
	lookat.x = x; lookat.y = y; lookat.z = z;
}


// ----------------------------------------------------------------- set_up_vector

inline void
tzICamera::set_up_vector(const Vector3D& u) {
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
tzICamera::set_exposure_time(const float exposure) {
	exposure_time = exposure;
}


#endif
