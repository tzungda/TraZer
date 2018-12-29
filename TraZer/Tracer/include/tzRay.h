#ifndef TZ_TRACER_RAY
#define TZ_TRACER_RAY

#include "Point3D.h"
#include "Vector3D.h"

class tzRay
{
public:
	Point3D o;		// origin
	Vector3D d;		// direction

	// default constructor
	tzRay( void );	

	// constructor
	tzRay( const Point3D& origin, const Vector3D& dir );

	// copy consturctor
	tzRay( const tzRay& ray );

	tzRay &operator = (const tzRay& rhs);

	~tzRay( void );
};

#endif
