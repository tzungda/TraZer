#ifndef TZ_TRACER_ICAMERA
#define TZ_TRACER_ICAMERA


#include "tzPoint3D.h"
#include "tzVector3D.h"
#include <string>
#include <memory>

class tzWorld;  

class tzICamera 
{
	public:
	
		tzICamera();   							// default constructor

		tzICamera(const tzICamera& tzICamera);			// copy constructor
		
		virtual std::shared_ptr<tzICamera> clone(void) const = 0;
		
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
	
		tzPoint3D		mEye;				// eye point
		tzPoint3D		mLookAt; 			// lookat point
		float			mRollAngle;					// roll angle
		tzVector3D		mOrthoU, mOrthoV, mOrthoW;			// orthonormal basis vectors
		tzVector3D		mUp;					// up vector
		float			mExposureTime;

		std::string		mOutputPath;
		
		tzICamera& operator= (const tzICamera& camera);
};


// inlined access functions
//
//===================================================================================
inline void tzICamera::setEye(const tzPoint3D& p) 
{
	mEye = p;
}

//===================================================================================
inline void tzICamera::setEye(const float x, const float y, const float z) 
{
	mEye.x = x; mEye.y = y; mEye.z = z;
}

//===================================================================================
inline void tzICamera::setLookAt(const tzPoint3D& p) 
{
	mLookAt = p;
}

//===================================================================================
inline void tzICamera::setLookAt(const float x, const float y, const float z)
{
	mLookAt.x = x; mLookAt.y = y; mLookAt.z = z;
}

//===================================================================================
inline void tzICamera::setUpVector(const tzVector3D& u) 
{
	mUp = u;
}

//===================================================================================
inline void tzICamera::setUpVector(const float x, const float y, const float z) 
{
	mUp.x = x; mUp.y = y; mUp.z = z;
}

//===================================================================================
inline void tzICamera::setRoll(const float r)
{
	mRollAngle = r;
}

//===================================================================================
inline void tzICamera::setExposureTime(const float exposure) 
{
	mExposureTime = exposure;
}


#endif
