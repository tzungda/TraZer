#include "../Include/AntTweakBar/AntTweakBar.h"
#include "../Include/Common.h"
#include "../Include/viewmanager.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Include/STB/stb_image_write.h"
#include "tree.h"

#if (_MSC_VER == 1600)
#include <string>  
#include <type_traits>  
namespace std {
	template<typename T>  
	typename enable_if<is_convertible<T, _Longlong>::value, string>::type to_string(T rhs) { return to_string(static_cast<_Longlong>(rhs)); }  
}
#endif

using namespace glm;
using namespace std;

#define SHADOW_OTHO	15.0f
#define POINT_LIGHT_NEARPLANE 0.1f
#define POINT_LIGHT_FARPLANE 50.0f
#define DIRECTIONAL_LIGHT_NEARPLANE 0.01f
#define DIRECTIONAL_LIGHT_FARPLANE 50.0f
#define SHADOWMAP_RESOLUTION 2048

struct
{
	GLint  model_matrix;
	GLint  view_matrix;
	GLint  projection_matrix;
	GLint  light_matrix;
	GLint  farPlane;
	GLint  lightSourceType;
	GLint  lightPos;
	GLint  viewPos;
	GLint  useShadowmap;
	GLint  useBias;
	GLint  pcfKernel;
	GLint  useInstancing;
	GLint  tilingFactor;
	GLint  treeStemFactor;
} blinnPhongShaderUniforms;

struct
{
	GLint  model_matrix;
	GLint  light_matrix;
	GLint  useInstancing;
} directionalLightDepthShaderUniforms;

struct
{
	GLint  model_matrix;
	GLint  farPlane;
	GLint  lightPos;
	GLint  shadow_matrices[6];
	GLint  useInstancing;
} pointLightDepthShaderUniforms;

GLuint			blinnPhongShadowShaderProgram;
GLuint			directionalLightDepthShaderProgram;
GLuint			pointLightDepthShaderProgram;

// Global variables

mat4			proj_matrix;
float			aspect;
vector<Shape>	m_shapes;
ViewManager		m_camera;
vec2			m_screenSize;
GLuint			m_mainWindow;
GLuint			m_quadVAO;
vec3			m_lightPos = vec3(25, 25, 0);
GLuint			m_directionalLight_depthMapFBO;
GLuint			m_pointLight_depthMapFBO;
GLuint			m_directionalLight_shadowDepthMap;
GLuint			m_pointLight_shadowDepthMap;
Shape			m_stemShape, m_appleShape;
Tree			m_tree;
TreeParams		m_treeParams;
TreeParams		m_normalTreeParams = { 60, 0.78, 0.6, 0.8, 0.016, 30, 0.6, 0.52, 0.17, 3, 16 };
TreeParams		m_willowParams = { 60, 0.78, 0.63, 0.8, 0.01, 16, 0.6, 0.52, 0.17, 2, 40 };

// GUI variables

TwBar			*bar;
bool			m_hideBar = false;
int				m_lightSource_type = 1;
int				m_last_lightSource_type = 0;
int				m_showLight = 0;
int				m_autoRotate = 0;
int				m_treeDepth = 5;
int				m_last_treeDepth = m_treeDepth;
int				m_appleDecorator = 1;
int				m_leafDecorator = 1;
int				m_treeType = 0;
int				m_last_treeType = m_treeType;

GLuint loadShader(string vsPath, string fsPath, string gsPath = "")
{
	bool useGeometricShader = gsPath != "";
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint gs = (useGeometricShader) ? gs = glCreateShader(GL_GEOMETRY_SHADER) : -1;
	char** vsSource = Common::LoadShaderSource(vsPath.c_str());
	char** fsSource = Common::LoadShaderSource(fsPath.c_str());
	char** gsSource = (useGeometricShader) ? Common::LoadShaderSource(gsPath.c_str()) : NULL;
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	if (useGeometricShader) glShaderSource(gs, 1, gsSource, NULL);
	Common::FreeShaderSource(vsSource);
	Common::FreeShaderSource(fsSource);
	if (useGeometricShader) Common::FreeShaderSource(gsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	if (useGeometricShader) glCompileShader(gs);
	Common::ShaderLog(vs);
	Common::ShaderLog(fs);
	if (useGeometricShader) Common::ShaderLog(gs);
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	if (useGeometricShader) glAttachShader(shaderProgram, gs);
	glLinkProgram(shaderProgram);
	return shaderProgram;
}
void renderQuad()
{
	if (m_quadVAO == 0)
	{
		float quadVertices[] =
		{
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		unsigned int quadVBO;
		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void takeScreenshotPNG()
{
	const unsigned int Width = m_screenSize.x;
	const unsigned int Height = m_screenSize.y;
	int size = Width * Height * 4;
	unsigned char *pixels = new unsigned char[size];
	unsigned char *rotatedPixels = new unsigned char[size];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	// invert Y axis
	for (int h = 0; h < Height; ++h)
	{
		for (int w = 0; w < Width; ++w)
		{
			rotatedPixels[((Height - h - 1) * Width + w) * 4] = pixels[(h * Width + w) * 4];
			rotatedPixels[((Height - h - 1) * Width + w) * 4 + 1] = pixels[(h * Width + w) * 4 + 1];
			rotatedPixels[((Height - h - 1) * Width + w) * 4 + 2] = pixels[(h * Width + w) * 4 + 2];
			rotatedPixels[((Height - h - 1) * Width + w) * 4 + 3] = pixels[(h * Width + w) * 4 + 3];
		}
	}

	int fileIndex = 0;
	while (1)
	{
		string fileName = string("./" + to_string(fileIndex) + ".png").c_str();
		std::ifstream infile(fileName);
		if (!infile.good())
			break;
		fileIndex++;
	}
	stbi_write_png(string("./" + to_string(fileIndex) + ".png").c_str(), Width, Height, 4, rotatedPixels, 0);
	delete pixels, rotatedPixels;
	printf("Take screenshot\n");
}
void TW_CALL SetAutoRotateCB(const void *value, void *clientData)
{
	m_autoRotate = *(const int *)value;
}
void TW_CALL GetAutoRotateCB(void *value, void *clientData)
{
	*(int *)value = m_autoRotate;
}
void TW_CALL SetShowLightCB(const void *value, void *clientData)
{
	m_showLight = *(const int *)value;
}
void TW_CALL GetShowLightCB(void *value, void *clientData)
{
	*(int *)value = m_showLight;
}
void TW_CALL SetShowAppleDecoratorCB(const void *value, void *clientData)
{
	m_appleDecorator = *(const int *)value;
	m_tree.setDrawingOption(m_leafDecorator, m_appleDecorator);
}
void TW_CALL GetShowAppleDecoratorCB(void *value, void *clientData)
{
	*(int *)value = m_appleDecorator;
}
void TW_CALL SetShowLeafDecoratorCB(const void *value, void *clientData)
{
	m_leafDecorator = *(const int *)value;
	m_tree.setDrawingOption(m_leafDecorator, m_appleDecorator);
}
void TW_CALL GetShowLeafDecoratorCB(void *value, void *clientData)
{
	*(int *)value = m_leafDecorator;
}
void checkLightSourceTypeChanged()
{
	if (m_last_lightSource_type != m_lightSource_type)
	{
		m_lightSource_type = m_last_lightSource_type;

		if (m_lightSource_type == 0) // directional light
		{
			m_lightPos = vec3(11.491428, 25.000000, 22.202332);
		}
		else if (m_last_lightSource_type == 1) // point light
		{
			m_lightPos = vec3(0, 2, 4);
		}
	}
}
void checkTreeDepthChanged()
{
	if (m_last_treeDepth != m_treeDepth)
	{
		m_treeDepth = m_last_treeDepth;
		m_tree.build(m_treeDepth);
	}
}
void checkTreeTypeChanged()
{
	if (m_last_treeType != m_treeType) 
	{
		m_treeType = m_last_treeType;

		if (m_treeType == 0)
		{
			m_treeParams = m_normalTreeParams;
			m_tree.setParams(m_treeParams);
			m_tree.setDrawingOption(m_leafDecorator, m_appleDecorator);
		}
		else if (m_treeType == 1)
		{
			m_treeParams = m_willowParams;
			m_tree.setParams(m_treeParams);
			m_tree.setDrawingOption(m_leafDecorator, false);
		}
		m_tree.build(m_treeDepth);
	}
}

void setupGUI()
{
	// Initialize AntTweakBar
#ifdef _MSC_VER
    TwInit(TW_OPENGL, NULL);
#else
    TwInit(TW_OPENGL_CORE, NULL);
#endif
	TwGLUTModifiersFunc(glutGetModifiers);
	bar = TwNewBar("Properties");
	TwDefine("Properties fontsize='3' size='260 200' color='0 0 0' alpha=128");  // http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:twbarparamsyntax
	TwAddVarCB(bar, "Show Light", TW_TYPE_BOOL32, SetShowLightCB, GetShowLightCB, NULL, "");
	TwAddVarCB(bar, "Auto Rotate", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL, "");
	TwAddVarCB(bar, "Apple Decorator", TW_TYPE_BOOL32, SetShowAppleDecoratorCB, GetShowAppleDecoratorCB, NULL, "");
	TwAddVarCB(bar, "Leaf Decorator", TW_TYPE_BOOL32, SetShowLeafDecoratorCB, GetShowLeafDecoratorCB, NULL, "");
	{
		TwEnumVal lightSourceTypeEV[2] = { { 0, "Directional Light" },{ 1, "Point Light" } };
		TwType type = TwDefineEnum("LightSourceType", lightSourceTypeEV, 2);
		TwAddVarRW(bar, "Light source type", type, &m_last_lightSource_type, "");
	}
	{
		TwEnumVal treeTypeEV[2] = { { 0, "Normal tree" },{ 1, "Willow" } };
		TwType type = TwDefineEnum("TreeType", treeTypeEV, 2);
		TwAddVarRW(bar, "Tree type", type, &m_last_treeType, "");
	}
	TwAddVarRW(bar, "Tree Depth", TW_TYPE_INT8, &m_last_treeDepth, "min='1' max='10' step='1' keyincr=ALT+F1 keydecr=ALT+F2 ");

	TwAddVarRW(bar, "Angle", TW_TYPE_FLOAT, &m_treeParams.angle, "group='TreeParams' min='0' max='359'");
	TwAddVarRW(bar, "Length Multiplier", TW_TYPE_FLOAT, &m_treeParams.lengthMult, "group='TreeParams' min='0' max='1' step='0.01'");
	TwAddVarRW(bar, "Thin Multiplier", TW_TYPE_FLOAT, &m_treeParams.thinMult, "group='TreeParams' min='0' max='1' step='0.01'");
	TwAddVarRW(bar, "Initial Length Factor", TW_TYPE_FLOAT, &m_treeParams.initLengthFac, "group='TreeParams' min='0' max='1' step='0.01'");
	TwAddVarRW(bar, "Initial Thin Factor", TW_TYPE_FLOAT, &m_treeParams.initThinFac, "group='TreeParams' min='0' max='1' step='0.01'");
	TwAddVarRW(bar, "Limb Angle", TW_TYPE_FLOAT, &m_treeParams.angleLimb, "group='TreeParams' min='0' max='359'");
	TwAddVarRW(bar, "Stem Factor", TW_TYPE_FLOAT, &m_treeParams.stemFac, "group='TreeParams' min='0' max='1' step='0.01'");
	TwAddVarRW(bar, "Limb Offset", TW_TYPE_FLOAT, &m_treeParams.limbOffset, "group='TreeParams' min='0' max='1' step='0.01'");
	TwAddVarRW(bar, "Leaf Size", TW_TYPE_FLOAT, &m_treeParams.leafSize, "group='TreeParams' min='0' step='0.01'");
	TwAddVarRW(bar, "Number of Limbs", TW_TYPE_INT32, &m_treeParams.numLimbs, "group='TreeParams' min='0' max='10'");
	TwAddVarRW(bar, "Max Leaves", TW_TYPE_INT32, &m_treeParams.maxLeaves, "group='TreeParams' min='0'");
}
void setupModels()
{
	int numModels = 3;
	string model_names[] =
	{
		"disk-ground.obj",
		"trunk.obj",
		"apple.obj",
	};

	int numTextures = 1;
	string texture_names[] =
	{
		"sand.png"
	};

	map<string, vector<tinyobj::shape_t>> model_cache;
	map<string, TextureData> texture_cache;

	for (int i = 0; i < numModels; i++)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		map<string, vector<tinyobj::shape_t>>::iterator found = model_cache.find(model_names[i]);
		if (found != model_cache.end())
		{
			shapes = found->second;
		}
		else
		{
			std::string err;

			bool ret = tinyobj::LoadObj(shapes, materials, err, model_names[i].c_str());
			if (err.size() > 0)
			{
				printf("Load Models Fail: %s\n", err.c_str());
				continue;
			}
			model_cache.insert(pair<string, vector<tinyobj::shape_t>>(model_names[i], shapes));
			printf("Load Models Success ! Shapes size %d Material size %d\n", shapes.size(), materials.size());
		}

		for (int j = 0; j < shapes.size(); ++j)
		{
			Shape m_shape;

			glGenVertexArrays(1, &m_shape.vao);
			glBindVertexArray(m_shape.vao);

			glGenBuffers(3, &m_shape.vbo);
			glGenBuffers(1, &m_shape.p_normal);
			glBindBuffer(GL_ARRAY_BUFFER, m_shape.vbo);
			glBufferData(GL_ARRAY_BUFFER, shapes[j].mesh.positions.size() * sizeof(float) + shapes[j].mesh.normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, shapes[j].mesh.positions.size() * sizeof(float), &shapes[j].mesh.positions[0]);
			glBufferSubData(GL_ARRAY_BUFFER, shapes[j].mesh.positions.size() * sizeof(float), shapes[j].mesh.normals.size() * sizeof(float), &shapes[j].mesh.normals[0]);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)(shapes[j].mesh.positions.size() * sizeof(float)));

			glBindBuffer(GL_ARRAY_BUFFER, m_shape.p_normal);
			glBufferData(GL_ARRAY_BUFFER, shapes[j].mesh.normals.size() * sizeof(float), shapes[j].mesh.normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_shape.vboTex);
			glBufferData(GL_ARRAY_BUFFER, shapes[j].mesh.texcoords.size() * sizeof(float), shapes[j].mesh.texcoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_shape.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[j].mesh.indices.size() * sizeof(unsigned int), shapes[j].mesh.indices.data(), GL_STATIC_DRAW);
			m_shape.materialId = shapes[j].mesh.material_ids[0];
			m_shape.indexCount = shapes[j].mesh.indices.size();

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
#if 1
			printf("Add object %s\n", shapes[j].name.c_str());
#else
			printf("Add object %s", shapes[j].name.c_str());
			if (shapes[j].mesh.material_ids.size() > 0)
				printf("\t- %s", materials[shapes[j].mesh.material_ids[0]].diffuse_texname.c_str());
			printf("\n");
#endif
			if (model_names[i] == "trunk.obj")
			{
				m_stemShape = m_shape;
			}
			else if (model_names[i] == "apple2.obj")
			{
				m_appleShape = m_shape;
			}
			else
			{
				m_shapes.push_back(m_shape);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < numTextures; ++i)
	{
		TextureData textureData;
		map<string, TextureData>::iterator found = texture_cache.find(texture_names[i]);
		if (found != texture_cache.end())
		{
			textureData = found->second;
		}
		else
		{
			textureData = Common::Load_png(texture_names[i].c_str());
			texture_cache.insert(pair<string, TextureData>(texture_names[i], textureData));
		}

		glGenTextures(1, &m_shapes[i].m_texture);
		glBindTexture(GL_TEXTURE_2D, m_shapes[i].m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_shapes[1].scale = vec3(1, 3, 1);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_tree = Tree(m_stemShape, m_appleShape);
	m_treeParams = m_normalTreeParams;
	m_tree.setParams(m_treeParams);
	m_tree.build(m_treeDepth);
}
void setupShaders()
{
	string dirPath = "13.4.fractal_trees/";

	blinnPhongShadowShaderProgram = loadShader(dirPath + "blinnPhong_shadow_instancing.vs", dirPath + "blinnPhong_shadow_instancing.fs");
	{
		glUseProgram(blinnPhongShadowShaderProgram);

		blinnPhongShaderUniforms.lightPos = glGetUniformLocation(blinnPhongShadowShaderProgram, "lightPos");
		blinnPhongShaderUniforms.viewPos = glGetUniformLocation(blinnPhongShadowShaderProgram, "viewPos");
		blinnPhongShaderUniforms.model_matrix = glGetUniformLocation(blinnPhongShadowShaderProgram, "model");
		blinnPhongShaderUniforms.view_matrix = glGetUniformLocation(blinnPhongShadowShaderProgram, "view");
		blinnPhongShaderUniforms.projection_matrix = glGetUniformLocation(blinnPhongShadowShaderProgram, "projection");
		blinnPhongShaderUniforms.light_matrix = glGetUniformLocation(blinnPhongShadowShaderProgram, "lightSpaceMatrix");
		blinnPhongShaderUniforms.useShadowmap = glGetUniformLocation(blinnPhongShadowShaderProgram, "useShadowMap");
		blinnPhongShaderUniforms.useBias = glGetUniformLocation(blinnPhongShadowShaderProgram, "useBias");
		blinnPhongShaderUniforms.pcfKernel = glGetUniformLocation(blinnPhongShadowShaderProgram, "pcfKernel");
		blinnPhongShaderUniforms.lightSourceType = glGetUniformLocation(blinnPhongShadowShaderProgram, "lightSourceType");
		blinnPhongShaderUniforms.farPlane = glGetUniformLocation(blinnPhongShadowShaderProgram, "farPlane");
		blinnPhongShaderUniforms.useInstancing = glGetUniformLocation(blinnPhongShadowShaderProgram, "useInstancing");
		blinnPhongShaderUniforms.tilingFactor = glGetUniformLocation(blinnPhongShadowShaderProgram, "tilingFactor");
		blinnPhongShaderUniforms.treeStemFactor = glGetUniformLocation(blinnPhongShadowShaderProgram, "treeStemFactor");

		glUniform1i(glGetUniformLocation(blinnPhongShadowShaderProgram, "diffuseTexture"), 0);
		glUniform1i(glGetUniformLocation(blinnPhongShadowShaderProgram, "shadowMap_directional"), 1);
		glUniform1i(glGetUniformLocation(blinnPhongShadowShaderProgram, "shadowMap_point"), 2);
	}

	directionalLightDepthShaderProgram = loadShader(dirPath + "directionalLight_depth.vs", dirPath + "directionalLight_depth.fs");
	{
		glUseProgram(directionalLightDepthShaderProgram);

		directionalLightDepthShaderUniforms.model_matrix = glGetUniformLocation(directionalLightDepthShaderProgram, "model");
		directionalLightDepthShaderUniforms.light_matrix = glGetUniformLocation(directionalLightDepthShaderProgram, "lightSpaceMatrix");
		directionalLightDepthShaderUniforms.useInstancing = glGetUniformLocation(directionalLightDepthShaderProgram, "useInstancing");

		glUniform1i(glGetUniformLocation(directionalLightDepthShaderProgram, "diffuseTexture"), 0);
	}

	pointLightDepthShaderProgram = loadShader(dirPath + "pointLight_depth.vs", dirPath + "pointLight_depth.fs", dirPath + "pointLight_depth.gs");
	{
		glUseProgram(pointLightDepthShaderProgram);

		pointLightDepthShaderUniforms.model_matrix = glGetUniformLocation(pointLightDepthShaderProgram, "model");
		pointLightDepthShaderUniforms.lightPos = glGetUniformLocation(pointLightDepthShaderProgram, "lightPos");
		pointLightDepthShaderUniforms.farPlane = glGetUniformLocation(pointLightDepthShaderProgram, "farPlane");
		pointLightDepthShaderUniforms.useInstancing = glGetUniformLocation(pointLightDepthShaderProgram, "useInstancing");
		for (int i = 0; i < 6; ++i)
		{
			pointLightDepthShaderUniforms.shadow_matrices[i] = glGetUniformLocation(pointLightDepthShaderProgram, string("shadowMatrices[" + to_string(i) + "]").c_str());
		}

		glUniform1i(glGetUniformLocation(pointLightDepthShaderProgram, "diffuseTexture"), 0);
	}
}

void My_Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	m_screenSize = vec2(800, 800);
	m_camera.SetWindowSize(800, 800);
	m_camera.SetZoom(3.0f);
	m_camera.SetRotation(0, deg2rad(30));
	m_camera.Translate(vec3(0, 50, -0.5));

	setupShaders();
	setupModels();

	glUseProgram(blinnPhongShadowShaderProgram);
	glUniform1i(blinnPhongShaderUniforms.useShadowmap, 1);
	glUniform1i(blinnPhongShaderUniforms.useBias, 1);
	glUniform1i(blinnPhongShaderUniforms.pcfKernel, 1);
	glUseProgram(0);

	// configure depth map FBO for directional light
	// -----------------------
	glGenFramebuffers(1, &m_directionalLight_depthMapFBO);
	// create depth texture
	glGenTextures(1, &m_directionalLight_shadowDepthMap);
	glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_directionalLight_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_directionalLight_shadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// configure depth map FBO for point light
	// -----------------------
	glGenFramebuffers(1, &m_pointLight_depthMapFBO);
	// create depth texture
	glGenTextures(1, &m_pointLight_shadowDepthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_pointLight_depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_pointLight_shadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void My_Display()
{
	glClearColor(1, 1, 1, 0);
	//glClearColor(0.486, 0.721, 0.918, 0.000); // 124, 184, 234
	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	/////////////////////////////////////////////////////////////////////////////////
	mat4 lightProjection, lightView, lightSpaceMatrix;
	lightProjection = glm::ortho(-SHADOW_OTHO, SHADOW_OTHO, -SHADOW_OTHO, SHADOW_OTHO, DIRECTIONAL_LIGHT_NEARPLANE, DIRECTIONAL_LIGHT_FARPLANE);
	lightView = glm::lookAt(m_lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	if (m_lightSource_type == 0) // directional light
	{
		glUseProgram(directionalLightDepthShaderProgram);
		{
			glUniformMatrix4fv(directionalLightDepthShaderUniforms.light_matrix, 1, GL_FALSE, value_ptr(lightSpaceMatrix));
			glViewport(0, 0, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);
			glBindFramebuffer(GL_FRAMEBUFFER, m_directionalLight_depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < m_shapes.size(); ++i)
			{
				Shape m_shape = m_shapes[i];

				glBindVertexArray(m_shape.vao);
				glUniformMatrix4fv(directionalLightDepthShaderUniforms.model_matrix, 1, GL_FALSE, value_ptr(m_shape.getTransformationMatrix()));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
				glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			m_tree.draw(directionalLightDepthShaderUniforms.model_matrix, directionalLightDepthShaderUniforms.useInstancing);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glUseProgram(0);
	}
	else if (m_lightSource_type == 1) // point light
	{
		mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOWMAP_RESOLUTION / (float)SHADOWMAP_RESOLUTION, POINT_LIGHT_NEARPLANE, POINT_LIGHT_FARPLANE);
		vector<mat4> shadowTransforms(6);
		shadowTransforms[0] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
		shadowTransforms[1] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
		shadowTransforms[2] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
		shadowTransforms[3] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
		shadowTransforms[4] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f));
		shadowTransforms[5] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f));

		glUseProgram(pointLightDepthShaderProgram);
		{
			for (int i = 0; i < 6; ++i)
			{
				glUniformMatrix4fv(pointLightDepthShaderUniforms.shadow_matrices[i], 1, GL_FALSE, value_ptr(shadowTransforms[i]));
			}
			glUniform1f(pointLightDepthShaderUniforms.farPlane, POINT_LIGHT_FARPLANE);
			glUniform3fv(pointLightDepthShaderUniforms.lightPos, 1, value_ptr(m_lightPos));

			glViewport(0, 0, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);
			glBindFramebuffer(GL_FRAMEBUFFER, m_pointLight_depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (int i = 0; i < m_shapes.size(); ++i)
			{
				Shape m_shape = m_shapes[i];

				glBindVertexArray(m_shape.vao);
				glUniformMatrix4fv(pointLightDepthShaderUniforms.model_matrix, 1, GL_FALSE, value_ptr(m_shape.getTransformationMatrix()));				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
				glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			m_tree.draw(pointLightDepthShaderUniforms.model_matrix, pointLightDepthShaderUniforms.useInstancing);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glUseProgram(0);
	}
	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	glUseProgram(blinnPhongShadowShaderProgram);
	{
		glUniform1i(blinnPhongShaderUniforms.lightSourceType, m_lightSource_type);
		glUniform1f(blinnPhongShaderUniforms.farPlane, POINT_LIGHT_FARPLANE);
		glUniform3fv(blinnPhongShaderUniforms.lightPos, 1, value_ptr(m_lightPos));
		glUniform3fv(blinnPhongShaderUniforms.viewPos, 1, value_ptr(m_camera.GetWorldEyePosition()));
		glUniformMatrix4fv(blinnPhongShaderUniforms.light_matrix, 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(blinnPhongShaderUniforms.view_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
		glUniformMatrix4fv(blinnPhongShaderUniforms.projection_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
		for (int i = 0; i < m_shapes.size(); ++i)
		{
			Shape m_shape = m_shapes[i];

			if (i == 0) glUniform1f(blinnPhongShaderUniforms.tilingFactor, 4);
			else glUniform1f(blinnPhongShaderUniforms.tilingFactor, 1);

			glBindVertexArray(m_shape.vao);
			glUniformMatrix4fv(blinnPhongShaderUniforms.model_matrix, 1, GL_FALSE, value_ptr(m_shape.getTransformationMatrix()));
			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
			glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		glUniform1f(blinnPhongShaderUniforms.tilingFactor, 1);
		m_tree.draw(blinnPhongShaderUniforms.model_matrix, blinnPhongShaderUniforms.useInstancing, blinnPhongShaderUniforms.treeStemFactor);
	}
	glUseProgram(0);
	/////////////////////////////////////////////////////////////////////////////////	
	if (m_showLight)
	{
		glPushMatrix();
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(value_ptr(m_camera.GetProjectionMatrix(aspect)));
			glColor4f(1.0, 1.0, 0.0, 1.0);
			if (m_lightSource_type == 0)
			{
				glLineWidth(5.0f);
				glBegin(GL_LINES);
				{
					glVertex3d(0, 0, 0);
					glVertex3d(m_lightPos.x, m_lightPos.y, m_lightPos.z);
				}
				glEnd();
			}
			else if (m_lightSource_type == 1)
			{
				glPointSize(20.0f);
				glBegin(GL_POINTS);
				{
					glVertex3d(m_lightPos.x, m_lightPos.y, m_lightPos.z);
				}
				glEnd();
			}
		}
		glPopMatrix();
	}
	/////////////////////////////////////////////////////////////////////////////////
	TwDraw();
	glutSwapBuffers();
	glutPostRedisplay();
}
void My_Reshape(int width, int height)
{
	printf("resize %d %d\n", width, height);
	
	m_screenSize = vec2(width, height);
	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);

	TwWindowSize(width, height);
}
void My_Timer(int val)
{
	if (m_autoRotate)
	{
		if (m_lightSource_type == 0)
			m_lightPos = glm::rotateY(m_lightPos, 1.0f * 0.01f);
		else
			m_lightPos = glm::rotateY(m_lightPos, 1.0f * 0.02f);		
	}

	checkLightSourceTypeChanged();
	checkTreeDepthChanged();
	checkTreeTypeChanged();

	glutPostRedisplay();
	glutTimerFunc(16, My_Timer, val);
}
void My_Mouse(int button, int state, int x, int y)
{
	if (!TwEventMouseButtonGLUT(button, state, x, y))
	{
		m_camera.mouseEvents(button, state, x, y);
	}
}
void My_Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) // Escape key
	{
		exit(0);
	}

	if (!TwEventKeyboardGLUT(key, x, y))
	{
		m_camera.keyEvents(key);

		if (key == 't' || key == 'T')
		{
			takeScreenshotPNG();
		}
		else if (key == 'g' || key == 'G')
		{
			m_tree.setParams(m_treeParams);
			m_tree.build(m_treeDepth);
		}
		else if (key == 'q' || key == 'Q')
		{
			m_lightPos = glm::rotate(m_lightPos, deg2rad(5), vec3(0, 1, 0));
		}
		else if (key == 'e' || key == 'E')
		{
			m_lightPos = glm::rotate(m_lightPos, deg2rad(-5), vec3(0, 1, 0));
		}
		else if (key == 'h')
		{
			printf("%f, %f, %f\n", m_lightPos.x, m_lightPos.y, m_lightPos.z);
		}
		else if (key == 'o' || key == 'O')
		{
			m_camera.ToggleOrtho();
		}
		else if (key == 'i' || key == 'I')
		{
			m_hideBar = !m_hideBar;
			if(m_hideBar) TwSetParam(bar, NULL, "visible", TW_PARAM_CSTRING, 1, "false");
			else TwSetParam(bar, NULL, "visible", TW_PARAM_CSTRING, 1, "true");
		}
	}
}
void My_SpecialKeys(int key, int x, int y)
{
	if (!TwEventSpecialGLUT(key, x, y))
	{
		switch (key)
		{

		}
	}
}
void My_Mouse_Moving(int x, int y)
{
	if (!TwEventMouseMotionGLUT(x, y))
	{
		m_camera.mouseMoveEvent(x, y);
	}
}

int main(int argc, char *argv[])
{
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);
	m_mainWindow = glutCreateWindow("13-4. Julia Fractal"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	Common::DumpInfo();
	////////////////////

	//Call custom initialize function
	My_Init();
	setupGUI();

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
	TwTerminate();
	return 0;
}