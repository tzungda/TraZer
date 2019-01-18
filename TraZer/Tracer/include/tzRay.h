#ifndef TZ_TRACER_RAY
#define TZ_TRACER_RAY

#include "tzPoint3D.h"
#include "tzVector3D.h"

class tzRay
{
public:
	tzPoint3D mOrigin;		// origin
	tzVector3D mDirection;		// direction
	int		mDepth;

	unsigned int mThreadId;
	unsigned int mMaxThreads;

	// default constructor
	tzRay( void );	

	// constructor
	tzRay( const tzPoint3D& origin, const tzVector3D& dir );

	// copy consturctor
	tzRay( const tzRay& ray );

	tzRay &operator = (const tzRay& rhs);

	~tzRay( void );
};

#endif
