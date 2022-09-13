
#include "Common.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyOBJ\tiny_obj_loader.h" //"../tools/tiny_obj_loader.h"

// core
#include "../Include/tzCoreScene.h"

// headers
#include "Tracer\include\tzWorld.h"

#include "../glView/include/tzGLWorldCentre.h"
#include "../glView/include/tzGLCamera.h"
#include "../glView/include/tzGLMesh.h"
#include "../glView/include/tzGLTexture.h"
#include "../glView/include/tzGLPhongMaterial.h"
#include "../glView/include/tzGLBlinnPhongMaterial_Shadow.h"
#include "../glView/include/tzGLPointLight.h"
#include "../glView/include/tzGLDirectionalLight.h"
#include "../glView/include/tzGLTools.h"
#include "../Include/tzTool.h"


int screenSizeX = 400;
int screenSizeY = 400;

struct
{
	GLint model_matrix;
	GLint view_matrix;
	GLint projection_matrix;
	GLint line_color;

} lineShaderPrograms;

GLuint			m_mainWindow;

// add meshes to scene
//-----------------------------------------------------------------------------------------------------------------
// create a scene
std::shared_ptr<tzCoreScene> myScene = nullptr;

tzGLCamera		m_camera;
std::map< std::string, std::shared_ptr<tzIGLMaterial> > g_glMaterial;
std::vector< std::shared_ptr<tzIGLLight> > g_glLights;

std::shared_ptr<tzCoreMesh> addMeshesToScene( std::shared_ptr<tzCoreScene> scene, const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t>& shapes)
{
	int numShapes = (int)shapes.size();
	for (int i = 0; i < numShapes; i++)
	{
		const tinyobj::shape_t shape = shapes[i];
		if ( shape.mesh.indices.size() % 3 != 0 || attrib.vertices.size() % 3 != 0)
		{
			printf(" mesh's indices needs to be a multiple of 3 \n");
			continue;
		}

		//
		std::shared_ptr<tzCoreMesh> newMesh = std::make_shared< tzCoreMesh >();
		newMesh->setName(shape.name);
		newMesh->setNumTriangles((int)shape.mesh.indices.size() / 3);

		// get uvs
		int numUVs = (int)attrib.texcoords.size() / 2;
		std::vector<float> us(numUVs); float* ptrU = &us[0];
		std::vector<float> vs(numUVs); float* ptrV = &vs[0];
		for (int uvid = 0; uvid < numUVs; uvid++)
		{
			int index = uvid * 2;
			(*ptrU++) = attrib.texcoords[index];
			(*ptrV++) = attrib.texcoords[index + 1];
		}
		newMesh->setUs(us);
		newMesh->setVs(vs);

		// get verts
		//
		bool isUvsMoreThanVerts = false;
		//
		int numVerts = (int)attrib.vertices.size() / 3;
		//
		std::vector<tzPoint3D> verts(numVerts); tzPoint3D *ptrP = &verts[0];
		for (int vid = 0; vid < numVerts; vid++)
		{
			int index = vid * 3;
			(*ptrP++) = tzPoint3D(attrib.vertices[index], attrib.vertices[index + 1], attrib.vertices[index + 2]);
		}
		newMesh->setNumVertices(numVerts);
		newMesh->setVertices(verts);

		// get normals
		bool isUvsMoreThanNorms = false;
		int numNorms = (int)attrib.normals.size() / 3;
		std::vector<tzNormal> normals(numNorms); tzNormal *ptrN = &normals[0];
		for (int nid = 0; nid < numNorms; nid++)
		{
			int index = nid * 3;
			(*ptrN++) = tzPoint3D(attrib.normals[index], attrib.normals[index + 1], attrib.normals[index + 2]);
		}
		newMesh->setVertexNormals(normals);

		// vertex faces & indices
		std::vector< std::vector< int > > vertexFaces(newMesh->numVertices());
		std::vector< std::vector< int > > faceVertices(newMesh->numTriangles());
		int idx = 0;
		for (int j = 0; j < newMesh->numTriangles(); j++)
		{
			int vid = shape.mesh.indices[idx].vertex_index;
			vertexFaces[vid].push_back(j);
			faceVertices[j].push_back(vid);
			idx++;

			vid = shape.mesh.indices[idx].vertex_index;
			vertexFaces[vid].push_back(j);
			faceVertices[j].push_back(vid);
			idx++;

			vid = shape.mesh.indices[idx].vertex_index;
			vertexFaces[vid].push_back(j);
			faceVertices[j].push_back(vid);
			idx++;
			//
		}
		newMesh->setFaceVertices(faceVertices);
		newMesh->setVertexFaces(vertexFaces);
		//
		std::vector< tzCoreMesh::index > indices(shape.mesh.indices.size());
		for (int ind = 0; ind < (int)indices.size(); ind++)
		{
			indices[ind].vertex_index = shape.mesh.indices[ind].vertex_index;
			indices[ind].normal_index = shape.mesh.indices[ind].normal_index;
			indices[ind].texcoord_index = shape.mesh.indices[ind].texcoord_index;
		}
		newMesh->setIndices(indices);

		// add new mesh
		scene->addMesh(newMesh);
		return newMesh;
	}
	return nullptr;
}

void addLightsToScene( std::shared_ptr<tzCoreScene> ptrScene)
{
	// create and add a point light
	std::shared_ptr<tzCoreLight> pointLight = std::make_shared< tzCoreLight >();
	pointLight->setPosition( tzVector3D(5.0f, 4.0f, 0.0f) );
	pointLight->setLightType( point );
	pointLight->setShadowmapResolution( 2048 );
	pointLight->setNearPlane( 0.1f );
	pointLight->setFarPlane( 50.0f );
	//
	ptrScene->addLight( pointLight );

	// create and add a directional light
	std::shared_ptr<tzCoreLight> directionalLight = std::make_shared< tzCoreLight >();
	directionalLight->setPosition(tzVector3D(0.001f, 15.0f, 0.0f));
	directionalLight->setLightType(directional);
	directionalLight->setShadowmapResolution(2048);
	directionalLight->setNearPlane( 0.1f );
	directionalLight->setFarPlane( 50.0f );
	//
	ptrScene->addLight( directionalLight );

}

// currently read the obj file with one mesh and one material
void loadObjToScene( std::shared_ptr<tzCoreScene> ptrScene, const std::string &objPath, const std::string &parentPath )
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objPath.c_str(), parentPath.c_str());
	if (err.size() > 0)
	{
		printf("Load Models Fail: %s\n", err.c_str());
		return;
	}

	std::shared_ptr<tzCoreMesh> newMesh = addMeshesToScene(ptrScene, attrib, shapes);
	//newMesh->setPosition( tzVector3D(10.0f, 0.0f, 0.0f ) );

	// create material and texture objects
	if ( materials.size() > 0 )
	{
		tinyobj::material_t mat = materials[0];
		
		// material
		std::shared_ptr<tzCoreMaterial> newMat = std::make_shared< tzCoreMaterial >();
		ptrScene->addMaterial( newMat );
		newMesh->setMaterial( newMat );
		newMat->setName( mat.name );
		newMat->setAmbientColor( tzVector3D( mat.ambient[0], mat.ambient[1], mat.ambient[2] ) );
		newMat->setDiffuseColor( tzVector3D(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]) );
		newMat->setSpecularColor( tzVector3D(mat.specular[0], mat.specular[1], mat.specular[2]) );
		//
		newMat->addFloatAttribute( "shininess", mat.shininess );
		newMat->addFloatAttribute( "ior", mat.ior );
		newMat->addFloatAttribute( "dissolve", mat.dissolve );
		newMat->addFloatAttribute( "illum", (float)mat.illum );

		// textures
		if ( mat.ambient_texname.length() > 0 )
		{
			std::shared_ptr<tzCoreTexture> newTex = std::make_shared< tzCoreTexture >();
			newTex->setPath(mat.ambient_texname);
			ptrScene->addTexture( newTex );
			newMat->addTexture( "ambient", newTex );
		}
		if (mat.diffuse_texname.length() > 0)
		{
			std::shared_ptr<tzCoreTexture> newTex = std::make_shared< tzCoreTexture >();
			newTex->setPath(mat.diffuse_texname);
			ptrScene->addTexture(newTex);
			newMat->addTexture("diffuse", newTex);
		}
		if (mat.specular_texname.length() > 0)
		{
			std::shared_ptr<tzCoreTexture> newTex = std::make_shared< tzCoreTexture >();
			newTex->setPath(mat.specular_texname);
			ptrScene->addTexture(newTex);
			newMat->addTexture("specular", newTex);
		}
	}

	
}

// update my GL objects
void updateGLObjects( const std::shared_ptr<tzCoreScene> ptrScene)
{
	const std::string shaderPath("./shaders/");

	// update materials
	std::shared_ptr<tzGLBlinnPhongMaterial_Shadow> newMat = std::make_shared< tzGLBlinnPhongMaterial_Shadow >();
	newMat->mPtrCamera = &m_camera;
	g_glMaterial["BlinnPhongMaterial_Shadow"] = newMat;

	// add lights
	const std::vector< std::shared_ptr<tzCoreLight> > lightList = ptrScene->lightList();
	for(std::vector< std::shared_ptr<tzCoreLight> >::const_iterator citer = lightList.begin(); citer != lightList.end(); citer++ )
	{
		if ( (*citer)->lightType() == point )
		{
			std::shared_ptr<tzGLPointLight> pointLight = std::make_shared< tzGLPointLight >();
			pointLight->setLightPos((*citer)->position());
			pointLight->setCoreObject( (*citer) );
			pointLight->setupShaders(shaderPath + "pointLight_depth.vs", shaderPath + "pointLight_depth.fs", shaderPath + "pointLight_depth.gs");
			g_glLights.push_back(pointLight);

			newMat->m_pointLight_shadowDepthMap = pointLight->shadowDepthMap();
			newMat->setPointLightPosition( pointLight->lightPos( ) );
		}
		else if ((*citer)->lightType() == directional )
		{
			std::shared_ptr<tzGLDirectionalLight> directionalLight = std::make_shared< tzGLDirectionalLight >();
			directionalLight->setLightPos((*citer)->position());
			directionalLight->setCoreObject((*citer));
			directionalLight->setupShaders(shaderPath + "directionalLight_depth.vs", shaderPath + "directionalLight_depth.fs");
			g_glLights.push_back( directionalLight );

			newMat->m_directionalLight_shadowDepthMap = directionalLight->shadowDepthMap();
			newMat->setDirectionalLightPosition( directionalLight->lightPos() );
		}
	}



	// update meshes
	for ( int i = 0; i < (int)ptrScene->meshList().size(); i++ )
	{
		// translation test
		//ptrScene->meshList()[i]->setPosition( tzVector3D( (float)i, 0.0f, 0.0f ) );
		//
		
		std::shared_ptr<tzGLMesh> newGlMesh = std::make_shared< tzGLMesh >();
		newGlMesh->setCoreObject( ptrScene->meshList()[i] );

		// add mesh to its material
		newMat->addMesh( newGlMesh );
		newGlMesh->setCoreMaterial(ptrScene->materialList()[i]);

		//newGlMesh->mPtrCoreMaterial = ptrScene->materialList()[i];
		newGlMesh->init();
		
		// add mesh to light 
		for ( std::vector< std::shared_ptr<tzIGLLight> >::iterator iter = g_glLights.begin(); iter != g_glLights.end(); iter++ )
		{
			(*iter)->addMesh( newGlMesh );
			(*iter)->addMesh(newGlMesh);
		}
	}

	// load shaders
	std::string dirPath = "./shaders/";
	for ( std::map< std::string, std::shared_ptr<tzIGLMaterial> >::iterator it = g_glMaterial.begin(); it != g_glMaterial.end(); it++ )
	{
		it->second->setupShaders(shaderPath + "blinnPhong_shadow.vs", shaderPath + "blinnPhong_shadow.fs");
	}
}
//-----------------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------
tzGLWorldCentre gWorldCentre;
GLuint			lineShaderProgram;
//---------------------------------------------------------------------------------------------------



//std::vector<tzGLMesh> g_Meshes;


void setupShaders()
{
	std::string dirPath = "./shaders/";
	
	// line shader
	glUseProgram(0);
	lineShaderProgram = tzGLTools::loadShader(dirPath + "line.vs", dirPath + "line.fs");
	{
		glUseProgram(lineShaderProgram);
		lineShaderPrograms.model_matrix = glGetUniformLocation(lineShaderProgram, "model");
		lineShaderPrograms.view_matrix = glGetUniformLocation(lineShaderProgram, "view");
		lineShaderPrograms.projection_matrix = glGetUniformLocation(lineShaderProgram, "projection");
		lineShaderPrograms.line_color = glGetUniformLocation(lineShaderProgram, "lineColor");
	}
}


void setupModels(std::shared_ptr<tzCoreScene> scene)
{
	// initialize world axes buffers
	gWorldCentre.init(lineShaderProgram);

	// path
	std::string dirPath = "./commonData/";

	//
	std::vector<std::string> model_names =
	{
		//dirPath+"trunk.obj",
		dirPath+"ladybug.obj",//"torus.obj",//"happynewyear.obj",//"sponza.obj",
		dirPath+"ladybug.obj",
		dirPath+"ladybug.obj",
		dirPath+"ground.obj",
		dirPath+"ground.obj",
		dirPath+"ground.obj",
		dirPath+"ground.obj",
		dirPath+"ground.obj",
		dirPath+"trunk.obj",
		dirPath+"leaves.obj"
	};

	
	// load geometiry file(obj) and its material
	for ( int i = 0; i < (int)model_names.size(); i++ )
	{
		loadObjToScene(scene, model_names[i], dirPath);
	}
	const std::vector<std::shared_ptr<tzCoreMesh>>& meshList = scene->meshList();
	// ladybugs
	meshList[0]->setPosition( tzVector3D( 5, 0, -5 ) );
	meshList[1]->setPosition( tzVector3D( -5, 0, -5 ) );
	meshList[1]->rotateY( 45.0f );
	meshList[1]->setScale( tzVector3D( 0.5f, 0.5f, 0.5f ) );
	meshList[2]->setPosition( tzVector3D( 6, 0, 1 ) );
	meshList[2]->rotateY( -30.0f );
	meshList[2]->setScale( tzVector3D( 0.2f, 0.2f, 0.2f ) );
	// grounds
	meshList[4]->setPosition( tzVector3D(0.0f, -20.0f, 10.0f) );
	meshList[4]->rotateX( -90.0f );
	meshList[5]->setPosition( tzVector3D(0.0f, 0.0f, -10.0f) );
	meshList[5]->rotateX( 90.0f );
	meshList[6]->setPosition( tzVector3D(10.0f, 0.0f, 0.0f) );
	meshList[6]->rotateZ( -90.0f );
	meshList[7]->setPosition( tzVector3D(-10.0f, 0.0f, 0.0f) );
	meshList[7]->rotateZ( 90.0f );

	// addLights to scene
	addLightsToScene( scene );

	// create the corresponding opengl objects for the viewport
	updateGLObjects( scene );

	return;
	

	//m_shapes[0].position = vec3(0, 0, 0);
}

void My_Init( std::shared_ptr<tzCoreScene> scene)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	//tzMatrix initMatrix = tzTool::lookAt( tzVector3D( 0.0f, 6.0f, 15.0f ), tzVector3D(0.0f, 0.0f, 0.0f), tzVector3D( 0.0f, 1.0f, 0.0f ) );
	m_camera.setCamPosition( tzVector3D(0.0f, 6.0f, 15.0f) );
	m_camera.setFarPlane( 1000.0f );
	//m_camera.Zoom(3.0f);

	// ooxx
	setupShaders();
	setupModels( scene );
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // 124, 184, 234
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tzMatrix view = m_camera.viewMatrix();//.invertedTransformMatrix();
	//tzVector3D pos(100.0f, 10.0f, 0.0f);

	// use point light
	int lightType = 1; // 0 for directional light, 1 for point light

	// 
	for ( std::vector< std::shared_ptr<tzIGLLight> >::iterator iter = g_glLights.begin(); iter != g_glLights.end(); iter++ )
	{
		(*iter)->updateAttributes();
	}


	// reset viewport
	glViewport(0, 0, screenSizeX, screenSizeY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 
	for (std::map< std::string, std::shared_ptr<tzIGLMaterial>>::iterator iter = g_glMaterial.begin(); iter != g_glMaterial.end(); iter++)
	{
		std::shared_ptr<tzGLBlinnPhongMaterial_Shadow> mat = std::dynamic_pointer_cast<tzGLBlinnPhongMaterial_Shadow>(iter->second);
		if (mat )
		{
			mat->mLightSourceType = lightType;
		}

		iter->second->updateAttributes( tzMatrix() );
	}

	
	glUseProgram(0);


	// world lines
	glUseProgram(lineShaderProgram);
	{
		tzMatrix mtx; // identical matrix
		glUniformMatrix4fv(lineShaderPrograms.view_matrix, 1, GL_FALSE, (GLfloat*)view.m);//value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
		glUniformMatrix4fv(lineShaderPrograms.projection_matrix, 1, GL_FALSE, (GLfloat*)(m_camera.projectionMatrix().m) );//value_ptr(m_camera.GetProjectionMatrix(aspect)));
		glUniformMatrix4fv(lineShaderPrograms.model_matrix, 1, GL_FALSE, (GLfloat*)(mtx.m));

		gWorldCentre.draw(lineShaderPrograms.line_color);
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

//Call to resize the window
void My_Reshape(int width, int height)
{
	//m_screenSize = vec2(width, height);
	screenSizeX = width;
	screenSizeY = height;

	m_camera.setWindowSize(width, height);
	glViewport(0, 0, width, height);

}

//Timer event
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(16, My_Timer, val);
}

//Mouse event
void My_Mouse(int button, int state, int x, int y)
{
	m_camera.mouseEvents(button, state, x, y);

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			//printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			//printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		//printf("Mouse %d is pressed\n", button);
	}
	//printf("%d %d %d %d\n",button,state,x,y);
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{
	m_camera.keyEvents(key);
	char k = 'r';
	//printf("Key %c is pressed at (%d, %d)\n", key, x, y);
	if ( key == k )
	{
		//
		printf( "--->Clicked key r to render \n" );
		tzWorld w;
		w.mScenePtr = myScene;
		w.build();
		printf("--->built the scene \n");
		w.setOutputPath("./renderImages/test2020.png");
		printf("--->started to render \n");
		w.renderScene();
		printf("--->render ended \n");
	}
}

//Special key event
void My_SpecialKeys(int key, int x, int y)
{
//	if (!TwEventSpecialGLUT(key, x, y))
	{
		switch (key)
		{
		case GLUT_KEY_F1:
			printf("F1 is pressed at (%d, %d)\n", x, y);
			break;
		case GLUT_KEY_PAGE_UP:
			printf("Page up is pressed at (%d, %d)\n", x, y);
			break;
		case GLUT_KEY_LEFT:
			printf("Left arrow is pressed at (%d, %d)\n", x, y);
			break;
		default:
			printf("Other special key is pressed at (%d, %d)\n", x, y);
			break;
		}
	}
}

void My_Mouse_Moving(int x, int y)
{
	m_camera.mouseMoveEvent(x, y);
}


int main(int argc, char *argv[])
{
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screenSizeX, screenSizeY);
	m_mainWindow = glutCreateWindow("TraZer - GL View"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	DumpInfo();
	////////////////////

	// create scene
	if( !myScene )
	{
		myScene = std::make_shared<tzCoreScene>();
	}

	//Call custom initialize function
	My_Init( myScene );

	//Register GLUT callback functions
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(16, My_Timer, 0);
	glutPassiveMotionFunc(My_Mouse_Moving);
	glutMotionFunc(My_Mouse_Moving);
	////////////////////

	// Enter main event loop.
	glutMainLoop();
	//TwTerminate();
	return 0;
}
