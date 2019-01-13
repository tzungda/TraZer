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
#include "../include/tzDirectional.h"
#include "../include/tzGrid.h"
#include "tzMesh.h"
#include "../include/tzImageTexture.h"
#include "../include/tzImage.h"
#include "../include/tzFlatUVMeshTriangle.h"
#include "../include/tzSmoothUVMeshTriangle.h"
#include "../include/tzMatteSV.h"

// core
#include "../include/tzCoreScene.h"

//
#include "../../Include/tzTool.h"

//
#include <algorithm>

void checkFileEnd( const char* filePath, const char* outputPath )
{
	char str[4096];
	char *result;

	FILE *fp = NULL;
	fopen_s( &fp, filePath, "r" );
	if ( !fp )
	{
		return;
	}

	bool needReplace = false;
	std::string outputStr;
	//
	result = fgets(str, 4096, fp);
	while (result != NULL )
	{
		std::string s( str );
		int p = (int)s.find( '\r' );
		if ( p >= 0 )
		{
			std::replace(s.begin(), s.end(), '\r', '\n');
			if (!needReplace )
			{
				needReplace = true;
			}
			outputStr += s;
		}
		result = fgets(str, 4096, fp);
	}

	//
	fclose( fp );
	if( !needReplace )
	{
		return;
	}

	// output new string
	FILE *fp_out = NULL;
	fopen_s(&fp_out, outputPath, "w");
	if (!fp_out)
	{
		fclose(fp);
		return;
	}
	fprintf( fp_out, outputStr.c_str() );
	fclose(fp_out);
}

/*
Constructor/Destructor
*/

//===================================================================================
tzWorld::tzWorld()
{
	mTracerPtr = NULL;
	mCameraPtr = NULL;
	mAmbientPtr = new tzAmbient();
	mScenePtr = NULL;
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
			(*iter) = NULL;
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
	float t;
	float tmin = kHugeValue;
	int numObjects = (int)mObjects.size();

	for ( int j = 0; j < numObjects; j++ )
	{
		if ( mObjects[j]->hit( ray, t, sr ) && ( t < tmin ) )
		{
			sr.mHitAnObject = true;
			tmin = t;
			sr.mColor = mObjects[j]->getColor();
		}
	}

	return sr;
}

//===================================================================================
void tzWorld::build()
{
	// area light & bunny------------------------------------------------------------------------------
	int numSamples = 16;

	tzISampler* sampler_ptr = new tzMultiJittered(numSamples);

	mVp.setHres(400);
	mVp.setVres(400);
	//mVp.setMaxDepth(0);
	mVp.setSampler(sampler_ptr);

	mBackgroundColor = tzRGBColor(0.5);

	mTracerPtr = new tzAreaLighting(this);

	tzPinhole* camera = new tzPinhole();
	camera->setEye(0, 0, -200);
	camera->setLookAt(0, 0, 0);
	camera->set_view_distance(16000);
	camera->computeUVW();
	setCamera(camera);


	tzEmissive* emissive_ptr = new tzEmissive;
	emissive_ptr->scaleRadiance(300);
	emissive_ptr->set_ce(white);

	//
	float width = 8.0f;				// for Figure 18.4(a) & (b)
	float height = 8.0f;
	//	float width = 2.0;				// for Figure 18.4(c)
	//	float height = 2.0;
	tzPoint3D center(0.0f, 7.0f, -7.0f);	// center of each area light (rectangular, disk, and spherical)
	tzPoint3D p0(-0.5f * width, center.y - 0.5f * height - 1.0f, center.z);
	tzVector3D a(width, 0.0, 0.0);
	tzVector3D b(0.0, height, 0.0);
	tzNormal normal(0, 0, 1);

	// rectangle emit object
	tzRectangle* rectangle_ptr = new tzRectangle(p0, a, b, normal);
	rectangle_ptr->setMaterial(emissive_ptr);
	rectangle_ptr->set_sampler(sampler_ptr);
	//rectangle_ptr->set_shadows(false);
	addObject(rectangle_ptr);

	// area light
	tzAreaLight* area_light_ptr = new tzAreaLight;
	area_light_ptr->setObject(rectangle_ptr);
	area_light_ptr->setCastsShadows(true);
	addLight(area_light_ptr);

	// point light
	tzPointLight* lightPtr = new tzPointLight();
	lightPtr->set_location(tzVector3D(0, 7, 7));
	lightPtr->scaleRadiance(0.2f);
	lightPtr->setCastsShadows(false);
	addLight(lightPtr);

	// material matte sv
	tzImage* image_ptr = new tzImage;
	const char* texPath = "C:\\Users\\User\\Desktop\\TraZer\\TraZer\\testImages\\images.ppm";
	//image_ptr->read_ppm_file(texPath);

	image_ptr->readPng( "C:\\Users\\User\\Desktop\\TraZer\\TraZer\\TraZer\\commonData\\torusTexture.png" );

	tzImageTexture* texture_ptr = new tzImageTexture;
	texture_ptr->set_image(image_ptr);

	tzMatteSV* sv_matte_ptr = new tzMatteSV;
	sv_matte_ptr->set_ka(0.1f);
	sv_matte_ptr->set_kd(4/*0.75*/);
	sv_matte_ptr->set_cd(texture_ptr);
	//tzMatte* mattePtr1 = new tzMatte();
	//mattePtr1->set_ka(0.25);
	//mattePtr1->set_kd(0.75);
	//mattePtr1->set_cd(0.4, 0.7, 0.4);

	//
	tzMatte* mattePtr2 = new tzMatte();
	mattePtr2->set_ka(0.1f);
	mattePtr2->set_kd(0.9f);
	mattePtr2->set_cd(white);

	//
	// "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Stanford_Bunny\\Bunny10K.ply"
	// "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Horse2K.ply"
	const char* file_name = "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Stanford_Bunny\\Bunny10K.ply";//"TwoTriangles.ply"; Horse2K
	//checkFileEnd(file_name, file_name);
	tzGrid* grid_ptr = new tzGrid(new tzMesh);
	grid_ptr->setScale(0.25f);
	if (mScenePtr && mScenePtr->meshList().size() > 0)
	{
		tzCoreMesh *ptrCoreMesh = mScenePtr->meshList()[0];
		grid_ptr->addMesh(ptrCoreMesh->vertices(), ptrCoreMesh->vertexNormals(), ptrCoreMesh->us(), ptrCoreMesh->vs(), ptrCoreMesh->vertexFaces(), ptrCoreMesh->indices(), ptrCoreMesh->numVertices(), ptrCoreMesh->numTriangles());
	}
	else
	{
		grid_ptr->read_smooth_uv_triangles((char*)file_name);		// for Figure 29.22(a)
																	//	grid_ptr->read_smooth_uv_triangles(file_name);		// for Figure 29.22(b)
	}
	//grid_ptr->read_flat_triangles((char*)file_name);
	grid_ptr->setMaterial(sv_matte_ptr);
	grid_ptr->setup_cells();
	addObject(grid_ptr);

	// sphere
	//tzSphere *spherePtr = new tzSphere(tzPoint3D(0, 2, 0), 1);
	//spherePtr->setMaterial(phongPtr);
	//addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(tzPoint3D(0, -1, 0), tzNormal(0, 1, 0));
	planePtr->setMaterial(mattePtr2);
	addObject(planePtr);
	

	/*----------------------------------------------------------------
	int numSamples = 16;

	mVp.setHres(400);
	mVp.setVres(400);
	mVp.setSamples(num_samples);

	mTracerPtr = new tzRayCast(this);

	mBackgroundColor = black;

	tzPinhole* pinhole_ptr = new tzPinhole;
	pinhole_ptr->setEye(80, 80, 80);
	pinhole_ptr->set_view_distance(1600.0);
	pinhole_ptr->setLookAt(0, -0.5, 0);
	pinhole_ptr->computeUVW();
	setCamera(pinhole_ptr);


	tzDirectional* directional_ptr = new tzDirectional;
	directional_ptr->setDirection(0.75, 1, -0.15);
	directional_ptr->scaleRadiance(4.5);
	directional_ptr->setCastsShadows(true);
	addLight(directional_ptr);

	tzImage* image_ptr = new tzImage;
	const char* texPath = "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\TextureImages\\TextureFiles\\ppm\\BlueGlass.ppm";
	image_ptr->read_ppm_file(texPath);

	tzImageTexture* texture_ptr = new tzImageTexture;
	texture_ptr->set_image(image_ptr);

	tzMatteSV* sv_matte_ptr = new tzMatteSV;
	sv_matte_ptr->set_ka(0.1);
	sv_matte_ptr->set_kd(0.75);
	sv_matte_ptr->set_cd(texture_ptr);

	const char* file_name = "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\Chapter29\\TwoUVTriangles.ply";//"TwoUVTriangles.ply";
	checkFileEnd(file_name, file_name);
	tzGrid* grid_ptr = new tzGrid(new tzMesh);

	if ( mScenePtr && mScenePtr->meshList().size() > 0 )
	{
		tzCoreMesh *ptrCoreMesh = mScenePtr->meshList()[0];
		grid_ptr->addMesh(ptrCoreMesh->vertices(), ptrCoreMesh->vertexNormals(), ptrCoreMesh->us(), ptrCoreMesh->vs(), ptrCoreMesh->vertexFaces(), ptrCoreMesh->faceVertices(), ptrCoreMesh->numVertices(), ptrCoreMesh->numTriangles() );
	}
	else
	{
		grid_ptr->read_smooth_uv_triangles((char*)file_name);		// for Figure 29.22(a)
														//	grid_ptr->read_smooth_uv_triangles(file_name);		// for Figure 29.22(b)
	}

	grid_ptr->setMaterial(sv_matte_ptr);
	grid_ptr->setup_cells();
	addObject(grid_ptr);


	tzMatte* matte_ptr = new tzMatte;
	matte_ptr->set_cd(1, 0.9, 0.6);
	matte_ptr->set_ka(0.25);
	matte_ptr->set_kd(0.4);

	tzPlane* plane_ptr1 = new tzPlane(tzPoint3D(0, -2.0, 0), tzNormal(0, 1, 0));
	plane_ptr1->setMaterial(matte_ptr);
	addObject(plane_ptr1);
	*/

	//--------------------------------------------------------------------------
	/*
	int numSamples = 16;

	mVp.setHres(400);
	mVp.setVres(400);
	mVp.setSamples(num_samples);

	mTracerPtr = new tzRayCast(this);

	mBackgroundColor = black;

	tzPinhole* pinhole_ptr = new tzPinhole;
	pinhole_ptr->setEye(0, 15, 15);
	pinhole_ptr->setLookAt(0, 0, 0);
	pinhole_ptr->set_view_distance(16000);
	pinhole_ptr->computeUVW();
	setCamera(pinhole_ptr);

	tzDirectional* directional_ptr = new tzDirectional;
	directional_ptr->setDirection(0.75, 1, -0.15);
	directional_ptr->scaleRadiance(4.5);
	directional_ptr->setCastsShadows(true);
	addLight(directional_ptr);

	tzMatte* matte_ptr1 = new tzMatte;
	matte_ptr1->set_ka(0.1);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(0.1, 0.5, 1.0);

	const char* file_name = "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Stanford_Bunny\\Bunny10K.ply";//"TwoTriangles.ply"; Horse2K
	checkFileEnd( file_name, file_name);
	tzGrid* grid_ptr = new tzGrid(new tzMesh);
	grid_ptr->read_flat_triangles((char*)file_name);		// for Figure 23.7(a)
													//	grid_ptr->read_smooth_triangles(file_name);		// for Figure 23.7(b)
	grid_ptr->setMaterial(matte_ptr1);
	grid_ptr->setup_cells();
	addObject(grid_ptr);

	tzMatte* matte_ptr2 = new tzMatte;
	matte_ptr2->set_cd(1, 0.9, 0.6);
	matte_ptr2->set_ka(0.25);
	matte_ptr2->set_kd(0.4);

	tzPlane* plane_ptr1 = new tzPlane(tzPoint3D(0, -2.0, 0), tzNormal(0, 1, 0));
	plane_ptr1->setMaterial(matte_ptr2);
	addObject(plane_ptr1);
	*/
	

	// area light & bunny------------------------------------------------------------------------------
	/*
	int numSamples = 100;

	tzISampler* sampler_ptr = new tzMultiJittered(num_samples);

	mVp.setHres(400);
	mVp.setVres(400);
	//mVp.setMaxDepth(0);
	mVp.setSampler(sampler_ptr);

	mBackgroundColor = tzRGBColor(0.5);

	mTracerPtr = new tzAreaLighting(this);

	tzPinhole* camera = new tzPinhole();
	camera->setEye(200, 200, 200);
	camera->setLookAt(0, 0, 0);
	camera->set_view_distance(16000);
	camera->computeUVW();
	setCamera(camera);


	tzEmissive* emissive_ptr = new tzEmissive;
	emissive_ptr->scaleRadiance(300);
	emissive_ptr->set_ce(white);

	//
	float width = 8.0;				// for Figure 18.4(a) & (b)
	float height = 8.0;
	//	float width = 2.0;				// for Figure 18.4(c)
	//	float height = 2.0;
	tzPoint3D center(0.0, 7.0, -7.0);	// center of each area light (rectangular, disk, and spherical)
	tzPoint3D p0(-0.5 * width, center.y - 0.5 * height - 1.0, center.z);
	tzVector3D a(width, 0.0, 0.0);
	tzVector3D b(0.0, height, 0.0);
	tzNormal normal(0, 0, 1);

	// point light
	//tzPointLight* lightPtr = new tzPointLight();
	//lightPtr->set_location(tzVector3D(50, 100, 0));
	//lightPtr->scaleRadiance(3.0);
	//lightPtr->setCastsShadows(true);
	//addLight(lightPtr);
	
	// rectangle emit object
	tzRectangle* rectangle_ptr = new tzRectangle(p0, a, b, normal);
	rectangle_ptr->setMaterial(emissive_ptr);
	rectangle_ptr->set_sampler(sampler_ptr);
	//rectangle_ptr->set_shadows(false);
	addObject(rectangle_ptr);

	// area light
	tzAreaLight* area_light_ptr = new tzAreaLight;
	area_light_ptr->setObject(rectangle_ptr);
	area_light_ptr->setCastsShadows(true);
	addLight(area_light_ptr);
	

	// material matte
	tzMatte* mattePtr1 = new tzMatte();
	mattePtr1->set_ka(0.25);
	mattePtr1->set_kd(0.75);
	mattePtr1->set_cd(0.4, 0.7, 0.4);
	//tzPhong* phongPtr = new tzPhong();
	//phongPtr->set_ka(0.25);
	//phongPtr->set_kd(0.75);
	//phongPtr->set_cd(0.4, 0.7, 0.4);  	// light green
	//phongPtr->set_ks(0.005);
	//phongPtr->set_exp(500);

	//
	tzMatte* mattePtr2 = new tzMatte();
	mattePtr2->set_ka(0.1);
	mattePtr2->set_kd(0.9);
	mattePtr2->set_cd(white);

	//
	// "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Stanford_Bunny\\Bunny10K.ply"
	// "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Horse2K.ply"
	const char* file_name = "C:\\Users\\User\\Desktop\\TraZer\\RayTraceGroundUp\\PLYFiles\\Stanford_Bunny\\Bunny10K.ply";//"TwoTriangles.ply"; Horse2K
	//checkFileEnd(file_name, file_name);
	tzGrid* grid_ptr = new tzGrid(new tzMesh);
	grid_ptr->setScale( 10.0f );
	grid_ptr->read_flat_triangles((char*)file_name);
	grid_ptr->setMaterial(mattePtr1);
	grid_ptr->setup_cells();
	addObject(grid_ptr);

	// sphere
	//tzSphere *spherePtr = new tzSphere(tzPoint3D(0, 2, 0), 1);
	//spherePtr->setMaterial(phongPtr);
	//addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(tzPoint3D(0, 0.5, 0), tzNormal(0, 1, 0));
	planePtr->setMaterial(mattePtr2);
	addObject(planePtr);
	*/

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
	aoPtr->scaleRadiance(1.0f);
	aoPtr->setColor( white );
	aoPtr->setMinAmount( 0.0f );
	aoPtr->setSampler( samplerPtr );
	setAmbientLight(aoPtr);

	// set camera
	tzPinhole *pinHolePtr = new tzPinhole();
	pinHolePtr->setEye(25, 20, 45);
	pinHolePtr->setLookAt(0, 1, 0);
	pinHolePtr->set_view_distance(5000);
	pinHolePtr->computeUVW();
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
	spherePtr->setMaterial(mattePtr1);
	addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(Point3D(0), Normal(0, 1, 0));
	planePtr->setMaterial(mattePtr2);
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
	ambientPtr->scaleRadiance( 1.0f );
	setAmbientLight( ambientPtr );

	// set camera
	tzPinhole *pinHolePtr = new tzPinhole();
	pinHolePtr->setEye(0, 200, 500);
	pinHolePtr->setLookAt(0, 0, 0);
	pinHolePtr->set_view_distance(850.0f );
	pinHolePtr->computeUVW();
	setCamera(pinHolePtr);

	// point light
	tzPointLight* lightPtr = new tzPointLight();
	lightPtr->set_location( Vector3D( 50, 100, 0) );
	lightPtr->scaleRadiance( 3.0 );
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
	spherePtr->setMaterial(phongPtr);
	addObject(spherePtr);

	// plane
	tzPlane* planePtr = new tzPlane(Point3D(0), Normal(0, 1, 0));
	planePtr->setMaterial(mattePtr);
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
	pinHolePtr->setEye(0, 0, 500);
	pinHolePtr->setLookAt(0);
	pinHolePtr->set_view_distance(500);
	pinHolePtr->computeUVW();
	setCamera(pinHolePtr );

	// background color
	mBackgroundColor = black;

	// create spheres
	tzSphere *spherePtr = new tzSphere();
	spherePtr->set_center( 0.0f, -25.0f, 0.0f );
	spherePtr->set_radius( 80 );
	spherePtr->setColor( 1.0f, 0.0f, 0.0f );
	addObject( spherePtr );
	//
	spherePtr = new tzSphere(Point3D(-45, 45, 40), 50);
	spherePtr->setColor( 1.0f, 1.0f, 0.0f );
	addObject( spherePtr );
*/
}

//===================================================================================
void tzWorld::renderScene() const
{
	if ( mCameraPtr )
	{
		mCameraPtr->setOutputPath(mOutputPath);	
		mCameraPtr->renderScene( *this );
		return;
	}
	

	std::vector< glm::vec4 > pixelColorArray;
	tzRGBColor pixelColor;
	tzRay ray;
	float zw = 100.0f;
//	float x, y;
	pixelColorArray.resize(mVp.mHres*mVp.mVres);

	ray.d = tzVector3D( 0.0f, 0.0f, -1.0f );
	const float h = (float)mVp.mHres;
	const float v = (float)mVp.mVres;
	tzPoint2D sp; // sample point in [0, 1] x [0, 1]
	tzPoint2D pp; // sample point on a pixel
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
				sp = mVp.mSamplerPtr->sampleUnitSquare();
				pp.x = mVp.mS * (fC - 0.5f*h + sp.x);
				pp.y = mVp.mS * (fR - 0.5f*v + sp.y);
				ray.o = tzPoint3D( pp.x, pp.y, zw );
				pixelColor += mTracerPtr->traceRay( ray );
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
void tzWorld::writeToBuffer(std::vector<glm::vec4>& buffer, const int row, const int column, const tzRGBColor& pixel_color) const
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
	float t;
	tzNormal normal;
	tzPoint3D localHitPoint;
	tmin = kHugeValue;
	int numObjects = (int)mObjects.size();

	//
	for ( int j = 0; j < numObjects; j++ )
	{
		if ( mObjects[j]->hit( ray, t, sr ) && ( t < tmin ) )
		{
			sr.mHitAnObject = true;
			tmin = (float)t;
			sr.mMaterialPtr = mObjects[j]->getMaterial();
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
