#include "../include/tzWorld.h"
#include "../include/tzSingleSphere.h"
#include "../include/tzMultipleObjects.h"
#include "../include/tzRay.h"
#include "../include/tzMultiJittered.h"
#include "../include/tzPinhole.h"
#include "../include/tzRayCast.h"
#include "../include/tzPointLight.h"
#include "../include/tzMatte.h"
#include "../include/tzPhong.h"
#include "../include/tzPlane.h"
#include "../include/tzAmbientOccluder.h"
#include "../include/tzAreaLight.h"
#include "../include/tzEmissive.h"
#include "../include/tzAreaLighting.h"
#include "../include/tzRectangle.h"

//
#include "../../Include/tzTool.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzWorld::tzWorld()
{
	mTracerPtr = NULL;
	mCameraPtr = NULL;
	mAmbientPtr = new tzAmbient();
}

//===================================================================================
tzWorld::~tzWorld()
{
	if (mTracerPtr)
	{
		delete mTracerPtr;
		mTracerPtr = NULL;
	}
	//
	for (std::vector< tzIGeometricObject* >::iterator iter = mObjects.begin(); iter != mObjects.end(); ++iter)
	{
		if ( (*iter) )
		{
			delete (*iter);
		}
	}
	mObjects.clear();
	//
	if ( mCameraPtr )
	{
		delete mCameraPtr;
		mCameraPtr = NULL;
	}

	//
	if (mAmbientPtr )
	{
		delete mAmbientPtr;
		mAmbientPtr = NULL;
	}
	//
	for (std::vector< tzILight* >::iterator iter = mLights.begin(); iter != mLights.end(); ++iter)
	{
		if ((*iter))
		{
			delete (*iter);
		}
	}
	mLights.clear();
}


/*
Interfaces
*/

//===================================================================================
tzShadeRec tzWorld::hitBareBonesObject(const tzRay &ray)
{
	tzShadeRec sr( *this );
	double t;
	double tmin = kHugeValue;
	int numObjects = (int)mObjects.size();

	for ( int j = 0; j < numObjects; j++ )
	{
		if ( mObjects[j]->hit( ray, t, sr ) && ( t < tmin ) )
		{
			sr.mHitAnObject = true;
			tmin = t;
			sr.mColor = mObjects[j]->get_color();
		}
	}

	return sr;
}

//===================================================================================
void tzWorld::build()
{
	int num_samples = 64;

	tzISampler* sampler_ptr = new tzMultiJittered(num_samples);

	mVp.setHres(600);
	mVp.setVres(600);
	//mVp.setMaxDepth(0);
	mVp.setSampler(sampler_ptr);

	mBackgroundColor = RGBColor(0.5);

	mTracerPtr = new tzAreaLighting(this);

	tzPinhole* camera = new tzPinhole();
	camera->set_eye(-20, 10, 20);
	camera->set_lookat(0, 2, 0);
	camera->set_view_distance(1080);
	camera->compute_uvw();
	setCamera(camera);


	tzEmissive* emissive_ptr = new tzEmissive;
	emissive_ptr->scale_radiance(300);
	emissive_ptr->set_ce(white);

	//
	float width = 8.0;				// for Figure 18.4(a) & (b)
	float height = 8.0;
	//	float width = 2.0;				// for Figure 18.4(c)
	//	float height = 2.0;
	Point3D center(0.0, 7.0, -7.0);	// center of each area light (rectangular, disk, and spherical)
	Point3D p0(-0.5 * width, center.y - 0.5 * height, center.z);
	Vector3D a(width, 0.0, 0.0);
	Vector3D b(0.0, height, 0.0);
	Normal normal(0, 0, 1);

	// rectangle emit object
	tzRectangle* rectangle_ptr = new tzRectangle(p0, a, b, normal);
	rectangle_ptr->set_material(emissive_ptr);
	rectangle_ptr->set_sampler(sampler_ptr);
	//rectangle_ptr->set_shadows(false);
	addObject(rectangle_ptr);

	// area light
	tzAreaLight* area_light_ptr = new tzAreaLight;
	area_light_ptr->set_object(rectangle_ptr);
	area_light_ptr->setCastsShadows(true);
	addLight(area_light_ptr);

	// material matte
	tzMatte* mattePtr1 = new tzMatte();
	mattePtr1->set_ka(0.25);
	mattePtr1->set_kd(0.75);
	mattePtr1->set_cd(0.4, 0.7, 0.4);
	//
	tzMatte* mattePtr2 = new tzMatte();
	mattePtr2->set_ka(0.1);
	mattePtr2->set_kd(0.9);
	mattePtr2->set_cd(white);

	// sphere
	tzSphere *spherePtr = new tzSphere(Point3D(0, 2, 0), 1);
	spherePtr->set_material(mattePtr1);
	addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(Point3D(0), Normal(0, 1, 0));
	planePtr->set_material(mattePtr2);
	addObject(planePtr);

	/*---------------------------------------------------------------------------------------------------------
	const int numSamples = 256;

	mVp.setHres(400);
	mVp.setVres(400);
	mVp.setPixelSize(1.0f);
	mVp.setSamples(numSamples);

	mBackgroundColor = black;
	mTracerPtr = new tzRayCast(this);

	// sampler
	tzMultiJittered *samplerPtr = new tzMultiJittered( numSamples );

	// ao
	tzAmbientOccluder *aoPtr = new tzAmbientOccluder();
	aoPtr->scale_radiance(1.0f);
	aoPtr->set_color( white );
	aoPtr->setMinAmount( 0.0f );
	aoPtr->setSampler( samplerPtr );
	setAmbientLight(aoPtr);

	// set camera
	tzPinhole *pinHolePtr = new tzPinhole();
	pinHolePtr->set_eye(25, 20, 45);
	pinHolePtr->set_lookat(0, 1, 0);
	pinHolePtr->set_view_distance(5000);
	pinHolePtr->compute_uvw();
	setCamera(pinHolePtr);

	// material matte
	tzMatte* mattePtr1 = new tzMatte();
	mattePtr1->set_ka(0.75);
	mattePtr1->set_kd(0.0);
	mattePtr1->set_cd(white);
	//
	tzMatte* mattePtr2 = new tzMatte();
	mattePtr2->set_ka(0.75);
	mattePtr2->set_kd(0.0);
	mattePtr2->set_cd(white);

	// sphere
	tzSphere *spherePtr = new tzSphere(Point3D(0, 1, 0), 1);
	spherePtr->set_material(mattePtr1);
	addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(Point3D(0), Normal(0, 1, 0));
	planePtr->set_material(mattePtr2);
	addObject(planePtr);
	*/

	/*------------------------------------------------------------------------------------
	mVp.setHres( 400 );
	mVp.setVres( 400 );
	mVp.setPixelSize( 1.0f );
	mVp.setSamples( 16 );

	mBackgroundColor = black;
	mTracerPtr = new tzRayCast(this);

	tzAmbient *ambientPtr = new tzAmbient();
	ambientPtr->scale_radiance( 1.0f );
	setAmbientLight( ambientPtr );

	// set camera
	tzPinhole *pinHolePtr = new tzPinhole();
	pinHolePtr->set_eye(0, 200, 500);
	pinHolePtr->set_lookat(0, 0, 0);
	pinHolePtr->set_view_distance(850.0f );
	pinHolePtr->compute_uvw();
	setCamera(pinHolePtr);

	// point light
	tzPointLight* lightPtr = new tzPointLight();
	lightPtr->set_location( Vector3D( 50, 100, 0) );
	lightPtr->scale_radiance( 3.0 );
	lightPtr->setCastsShadows( true );
	addLight( lightPtr );

	// material phong
	tzPhong* phongPtr = new tzPhong();
	phongPtr->set_ka(0.25);
	phongPtr->set_kd(0.75);
	phongPtr->set_cd(0.75, 0.75, 0);  	// dark yellow
	phongPtr->set_ks(0.25);
	phongPtr->set_exp(50);

	// material matte
	tzMatte* mattePtr = new tzMatte();
	mattePtr->set_ka(0.20);
	mattePtr->set_kd(0.97);
	mattePtr->set_cd(white);

	// sphere
	tzSphere *spherePtr = new tzSphere(Point3D(0, 50, 0), 25);
	spherePtr->set_material(phongPtr);
	addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(Point3D(0), Normal(0, 1, 0));
	planePtr->set_material(mattePtr);
	addObject(planePtr);
	*/
	
	/*----------------------------------------------------------------------------------------------
	// viewport plane
	mVp.setHres( 256 );
	mVp.setVres( 256 );
	mVp.setPixelSize( 1.0f );
	mVp.setGamma( 1.0f );
	const int numSamples = 25;
	mVp.setSampler( new tzMultiJittered(numSamples) );

	// tracer
	mTracerPtr = new tzMultipleObjects(this);

	// set camera
	tzPinhole *pinHolePtr = new tzPinhole();
	pinHolePtr->set_eye(0, 0, 500);
	pinHolePtr->set_lookat(0);
	pinHolePtr->set_view_distance(500);
	pinHolePtr->compute_uvw();
	setCamera(pinHolePtr );

	// background color
	mBackgroundColor = black;

	// create spheres
	tzSphere *spherePtr = new tzSphere();
	spherePtr->set_center( 0.0f, -25.0f, 0.0f );
	spherePtr->set_radius( 80 );
	spherePtr->set_color( 1.0f, 0.0f, 0.0f );
	addObject( spherePtr );
	//
	spherePtr = new tzSphere(Point3D(-45, 45, 40), 50);
	spherePtr->set_color( 1.0f, 1.0f, 0.0f );
	addObject( spherePtr );
*/
}

//===================================================================================
void tzWorld::renderScene() const
{
	if ( mCameraPtr )
	{
		mCameraPtr->setOutputPath(mOutputPath);	
		mCameraPtr->render_scene( *this );
		return;
	}
	

	std::vector< glm::vec4 > pixelColorArray;
	RGBColor pixelColor;
	tzRay ray;
	double zw = 100.0;
//	double x, y;
	pixelColorArray.resize(mVp.mHres*mVp.mVres);

	ray.d = Vector3D( 0.0f, 0.0f, -1.0f );
	const float h = (float)mVp.mHres;
	const float v = (float)mVp.mVres;
	Point2D sp; // sample point in [0, 1] x [0, 1]
	Point2D pp; // sample point on a pixel
	float invNumSamples = 1.0f/(float)mVp.mNumSamples;
	for ( int r = 0; r < mVp.mVres; r++ )
	{
		for ( int c = 0; c < mVp.mHres; c++ )
		{
			float fC = (float)c;
			float fR = (float)r;
			
			//x = mVp.mS * ( fC - 0.5f * ( h - 1.0f ) );
			//y = mVp.mS * ( fR - 0.5f * ( v - 1.0f ) );

			pixelColor = black;
			for ( int j = 0; j < mVp.mNumSamples; j++ )
			{
				sp = mVp.mSamplerPtr->sample_unit_square();
				pp.x = mVp.mS * (fC - 0.5f*h + sp.x);
				pp.y = mVp.mS * (fR - 0.5f*v + sp.y);
				ray.o = Point3D( pp.x, pp.y, zw );
				pixelColor += mTracerPtr->trace_ray( ray );
			}
			pixelColor *= invNumSamples;

			// 
			int index = c + r*mVp.mHres;
			pixelColorArray[index].r = pixelColor.r;
			pixelColorArray[index].g = pixelColor.g;
			pixelColorArray[index].b = pixelColor.b;
			pixelColorArray[index].a = 1.0f;
		}
	}

	//
	tzTool::writePng(pixelColorArray, mVp.mHres, mVp.mVres, (std::string)mOutputPath);
}

//===================================================================================
void tzWorld::openWindow(const int hres, const int vres) const
{
}

//===================================================================================
void tzWorld::writeToBuffer(std::vector<glm::vec4>& buffer, const int row, const int column, const RGBColor& pixel_color) const
{
	int index = column + row*mVp.mHres;
	buffer[index].r = pixel_color.r;
	buffer[index].g = pixel_color.g;
	buffer[index].b = pixel_color.b;
	buffer[index].a = 1.0f;
}

//===================================================================================
void tzWorld::writeImage( const std::vector<glm::vec4>& buffer, const std::string &imagePath) const
{
	tzTool::writePng(buffer, mVp.mHres, mVp.mVres, (std::string)imagePath);
}

//===================================================================================
void tzWorld::setCamera(const tzICamera *cam)
{
	mCameraPtr = (tzICamera*)cam;
}

//===================================================================================
void tzWorld::addLight(tzILight *lightPtr)
{
	mLights.push_back( lightPtr );
}

//===================================================================================
tzShadeRec tzWorld::hitObjects(const tzRay &ray, float &tmin) 
{
	tzShadeRec sr( *this );
	double t;
	Normal normal;
	Point3D localHitPoint;
	tmin = kHugeValue;
	int numObjects = (int)mObjects.size();

	//
	for ( int j = 0; j < numObjects; j++ )
	{
		if ( mObjects[j]->hit( ray, t, sr ) && ( t < tmin ) )
		{
			sr.mHitAnObject = true;
			tmin = (float)t;
			sr.mMaterialPtr = mObjects[j]->get_material();
			sr.mHitPoint = ray.o + tmin * ray.d;
			normal = sr.mNormal;
			localHitPoint = sr.mLocalHitPoint;
		}
	}

	//
	if ( sr.mHitAnObject )
	{
		sr.mT = tmin;
		sr.mNormal = normal;
		sr.mLocalHitPoint = localHitPoint;
	}

	return sr;
}

//===================================================================================
void tzWorld::setAmbientLight(tzILight *ambientLight)
{
	mAmbientPtr = ambientLight;
}

//===================================================================================
void tzWorld::setOutputPath(const std::string &outputPath)
{
	mOutputPath = outputPath;
}
