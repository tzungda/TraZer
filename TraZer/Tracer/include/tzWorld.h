#ifndef TZ_TRACER_WORLD
#define TZ_TRACER_WORLD

#include "../include/tzViewPlane.h"
#include "../include/tzSphere.h"
#include "../include/tzITracer.h"
#include "../include/tzICamera.h"
#include "../include/tzAmbient.h"
#include <vector>

//class tzILight;
class tzCoreScene;

class tzWorld
{
public:
	tzViewPlane							mVp;
	tzColor							mBackgroundColor;
	tzSphere							mSphere;
	tzITracer*							mTracerPtr;
	std::vector<tzIGeometricObject*>	mObjects;

	//
	tzICamera							*mCameraPtr;

	//
	tzILight							*mAmbientPtr;
	std::vector<tzILight*>				mLights;

	//
	std::string							mOutputPath;

	// scene
	tzCoreScene							*mScenePtr;


public:
	tzWorld();
	virtual ~tzWorld();

public:

	void				addObject(tzIGeometricObject *objectPtr);
	tzShadeRec			hitBareBonesObject( const tzRay &ray );

	void				build();
	void				renderScene() const;
	void				openWindow( const int hres, const int vres ) const;
	void				writeToBuffer( std::vector<tzColor>& buffer, const int row, const int column, const tzColor& pixel_color) const;
	void				writeImage( const std::vector<tzColor>& buffer, const std::string &imagePath ) const;

	// 
	void				setCamera( const tzICamera *cam );

	//
	void				addLight( tzILight *lightPtr );

	// 
	tzShadeRec			hitObjects( const tzRay &ray, float &tmin ) ;

	//
	void				setAmbientLight( tzILight *ambientLight );

	//
	void				setOutputPath( const std::string &outputPath );

};

//===================================================================================
inline void tzWorld::addObject(tzIGeometricObject *objectPtr)
{
	mObjects.push_back(objectPtr);
}

#endif
