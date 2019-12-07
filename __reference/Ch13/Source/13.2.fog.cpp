#include "../Include/AntTweakBar/AntTweakBar.h"
#include "../Include/Common.h"
#include "../Include/viewmanager.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Include/STB/stb_image_write.h"

using namespace glm;
using namespace std;

#if (_MSC_VER == 1600)
#include <string>  
#include <type_traits>  
namespace std {
	template<typename T>  
	typename enable_if<is_convertible<T, _Longlong>::value, string>::type to_string(T rhs) { return to_string(static_cast<_Longlong>(rhs)); }  
}
#endif

#define SHADOW_OTHO	15.0f
#define POINT_LIGHT_NEARPLANE 0.1f
#define POINT_LIGHT_FARPLANE 50.0f
#define DIRECTIONAL_LIGHT_NEARPLANE 0.01f
#define DIRECTIONAL_LIGHT_FARPLANE 50.0f
#define SHADOWMAP_RESOLUTION 1024

#define FOG_SCENE 1 // 0 for atmospheric scene, 1 for fog scene

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
	GLint  pcfKernel;
	GLint  fogColor;
	GLint  fogType;
	GLint  fogStart;
	GLint  fogEnd;
	GLint  fogDensity;
	GLint  bi_be_factor;
} blinnPhongShaderUniforms;

struct
{
	GLint  model_matrix;
	GLint  light_matrix;
} directionalLightDepthShaderUniforms;

struct
{
	GLint  model_matrix;
	GLint  farPlane;
	GLint  lightPos;
	GLint  shadow_matrices[6];
} pointLightDepthShaderUniforms;

GLuint			blinnPhongShaderProgram;
GLuint			directionalLightDepthShaderProgram;
GLuint			pointLightDepthShaderProgram;

mat4			proj_matrix;
float			aspect;
vector<Shape>	m_shapes;
ViewManager		m_camera;
vec3			m_lightPos;
GLuint			m_directionalLight_depthMapFBO;
GLuint			m_pointLight_depthMapFBO;
GLuint			m_directionalLight_shadowDepthMap;
GLuint			m_pointLight_shadowDepthMap;
vec2			m_screenSize;
int				m_lightSource_type = 1;
int				m_last_lightSource_type = 0;
int				m_showLight = 1;
int				m_autoRotate = 1;
TwBar			*bar;
int				fogType = 0;
float			fogDensity = 0.2;
float			fogStart = 1;
float			fogEnd = 6.0;
vec2			bi_be_factor = vec2(0.001, 0.004);
vec4			fogColor = vec4(0.5, 0.5, 0.5, 1.0);

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
void checkLightSourceTypeChanged()
{
	if (m_last_lightSource_type != m_lightSource_type)
	{
		m_lightSource_type = m_last_lightSource_type;

		if (m_lightSource_type == 0) // directional light
		{
			m_lightPos = vec3(25, 25, 0);
		}
		else if (m_last_lightSource_type == 1) // point light
		{
			m_lightPos = vec3(0, 3, 2.5);
		}
	}
}
GLuint loadShader(string vsPath, string fsPath, string gsPath = "")
{
	bool useGeometricShader = gsPath != "";
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint gs = (useGeometricShader) ? gs = glCreateShader(GL_GEOMETRY_SHADER) : -1;
	char** vsSource = Common::LoadShaderSource(vsPath.c_str()); //cout << *vsSource << endl;
	char** fsSource = Common::LoadShaderSource(fsPath.c_str()); //cout << *fsSource << endl;
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
void takeScreenshotPNG(string path)
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
	stbi_write_png(path.c_str(), Width, Height, 4, rotatedPixels, 0);
	delete pixels, rotatedPixels;
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
	TwDefine("Properties fontsize='3' size='320 180' position='20 590' color='0 0 0' alpha=128 valueswidth=fit");  // http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:twbarparamsyntax
	TwAddVarCB(bar, "Show Light", TW_TYPE_BOOL32, SetShowLightCB, GetShowLightCB, NULL, "");
	TwAddVarCB(bar, "Auto Rotate", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL, "");
	{
		TwEnumVal lightSourceTypeEV[2] = { { 0, "Directional Light" },{ 1, "Point Light" } };
		TwType type = TwDefineEnum("LightSourceType", lightSourceTypeEV, 2);
		TwAddVarRW(bar, "Light source type", type, &m_last_lightSource_type, "");
	}
	{
		TwEnumVal fogTypeEV[5] = { { 0, "None" },{ 1, "Linear" },{ 2, "Exp" },{ 3, "Exp Square" },{ 4,"Atmospheric Effects" } };
		TwType type = TwDefineEnum("fog Type", fogTypeEV, 5);
		TwAddVarRW(bar, "fog Type", type, &fogType, "group='Fog'");
	}
	{		
		TwAddVarRW(bar, "FogDensity", TW_TYPE_FLOAT, &fogDensity, "group='Fog' label='Fog Density'  min=0 max=1 step=0.01 help='Fog Density ( 0 ~ 1 )\nOnly work for type[Exp] [Exp Square]' ");
		TwAddVarRW(bar, "FogStart", TW_TYPE_FLOAT, &fogStart, "group='Fog' label='Fog Start' min=0 max=1000 step=0.1 help='Fog Start Distance ( 0 ~ 1000 ) Used to Control the Start Distance of Fog\nOnly Work for type [Linear] ' ");
		fogEnd = fogEnd < fogStart ? fogStart : fogEnd;
		TwAddVarRW(bar, "FogEnd", TW_TYPE_FLOAT, &fogEnd, "group='Fog' label='Fog End' min=0 max=1000 step=0.1 help='Fog End Distance ( 0 ~ 1000 ) Used to Control the End Distance of Fog\nOnly Work for type [Linear]' ");
		TwAddVarRW(bar, "Fog Color", TW_TYPE_COLOR3F, &fogColor, " group='Fog' label='Fog Color' help='Fog Color rgb' ");
		TwAddVarRW(bar, "Be factor", TW_TYPE_FLOAT, &bi_be_factor.y, " group='Fog' label='Be factor' min='0.0001' max='0.005' step='0.0001' help='\nOnly Work for type [Atmospheric Effects]' ");
		TwAddVarRW(bar, "Bi factor", TW_TYPE_FLOAT, &bi_be_factor.x, " group='Fog' label='Bi factor' min='0.0001' max='0.005' step='0.0001' help='\nOnly Work for type [Atmospheric Effects]' ");
	}
}
void setupModels()
{
	bool enableAutoTextureDetection = true;

#if FOG_SCENE
	int numMaterials = 15;
#else
	int numMaterials = 1;
#endif
	string material_basePaths[] =
	{
#if FOG_SCENE
		"dabrovic-sponza/",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./",
		"./"
#else
		"./"
#endif
	};

#if FOG_SCENE
	int numModels = 15;
#else
	int numModels = 1;
#endif
	string model_names[] =
	{
#if FOG_SCENE
		"dabrovic-sponza/sponza.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj",
		"teapot.obj"
#else
		"terrain.obj"
#endif
	};

	map<string, vector<Shape>> shape_cache;

	for (int i = 0; i < numModels; i++)
	{
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		map<string, vector<Shape>>::iterator found = shape_cache.find(model_names[i]);
		if (found != shape_cache.end())
		{
			vector<Shape> shapes = found->second;
			for (int j = 0; j < shapes.size(); ++j)
			{
				m_shapes.push_back(shapes[j]);
			}
		}
		else
		{
			std::string err;
			bool ret = tinyobj::LoadObj(shapes, materials, err, model_names[i].c_str(), material_basePaths[i].c_str());
			if (err.size() > 0)
			{
				printf("Load Models Fail: %s\n", err.c_str());
				continue;
			}
			printf("Load Models Success ! Shapes size %d Material size %d\n", shapes.size(), materials.size());

			vector<Shape> shapeBuffer;
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

				if (enableAutoTextureDetection)
				{
					string textureFileName;
					if (shapes[j].mesh.material_ids.size() > 0 && shapes[j].mesh.material_ids[0] >= 0)
					{
						textureFileName = materials[shapes[j].mesh.material_ids[0]].diffuse_texname.c_str();
					}
					else
					{
						textureFileName = "";
					}

					TextureData textureData;
					textureData = Common::Load_png((material_basePaths[i] + textureFileName).c_str());
					
					glGenTextures(1, &m_shape.m_texture);
					glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				shapeBuffer.push_back(m_shape);
				m_shapes.push_back(m_shape);
			}
			shape_cache.insert(pair<string, vector<Shape>>(model_names[i], shapeBuffer));
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if FOG_SCENE
	for (int i = 0; i < 5; i++) // right side teapots
	{
		int id = i * 2 + 33;
		m_shapes[id].position = vec3(-10 + 5 * i, 0, 5);
		m_shapes[id + 1].position = vec3(-10 + 5 * i, 0, 5);
		m_shapes[id].rotation = vec3(0, 45, 0);
		m_shapes[id + 1].rotation = vec3(0, 45, 0);
		m_shapes[id].scale = vec3(0.5);
		m_shapes[id + 1].scale = vec3(0.5);
	}
	for (int i = 0; i < 5; i++) // left side teapots
	{
		int id = i * 2 + 33 + 10;
		m_shapes[id].position = vec3(-10 + 5 * i, 0, -5);
		m_shapes[id + 1].position = vec3(-10 + 5 * i, 0, -5);
		m_shapes[id].rotation = vec3(0, -45, 0);
		m_shapes[id + 1].rotation = vec3(0, -45, 0);
		m_shapes[id].scale = vec3(0.5);
		m_shapes[id + 1].scale = vec3(0.5);
	}
	for (int i = 0; i < 4; i++) // middle side teapots
	{
		int id = i * 2 + 33 + 10 + 10;
		m_shapes[id].position = vec3(10 - 5 * i, 0, 0);
		m_shapes[id + 1].position = vec3(10 - 5 * i, 0, 0);
		m_shapes[id].rotation = vec3(0, 0, 0);
		m_shapes[id + 1].rotation = vec3(0, 0, 0);
		m_shapes[id].scale = vec3(0.5);
		m_shapes[id + 1].scale = vec3(0.5);
	}
#else
	m_shapes[0].position = vec3(50, 0, 0);
	m_shapes[0].rotation = vec3(0, -90, 0);
	m_shapes[0].scale = vec3(2, 2, 2);
#endif
}
void setupShaders()
{
	string dirPath = "13.2.fog/";

	blinnPhongShaderProgram = loadShader(dirPath + "blinnPhong_fog.vs", dirPath + "blinnPhong_fog.fs");
	{
		glUseProgram(blinnPhongShaderProgram);

		blinnPhongShaderUniforms.lightPos = glGetUniformLocation(blinnPhongShaderProgram, "lightPos");
		blinnPhongShaderUniforms.viewPos = glGetUniformLocation(blinnPhongShaderProgram, "viewPos");
		blinnPhongShaderUniforms.model_matrix = glGetUniformLocation(blinnPhongShaderProgram, "model");
		blinnPhongShaderUniforms.view_matrix = glGetUniformLocation(blinnPhongShaderProgram, "view");
		blinnPhongShaderUniforms.projection_matrix = glGetUniformLocation(blinnPhongShaderProgram, "projection");
		blinnPhongShaderUniforms.light_matrix = glGetUniformLocation(blinnPhongShaderProgram, "lightSpaceMatrix");
		blinnPhongShaderUniforms.pcfKernel = glGetUniformLocation(blinnPhongShaderProgram, "pcfKernel");
		blinnPhongShaderUniforms.lightSourceType = glGetUniformLocation(blinnPhongShaderProgram, "lightSourceType");
		blinnPhongShaderUniforms.farPlane = glGetUniformLocation(blinnPhongShaderProgram, "farPlane");

		blinnPhongShaderUniforms.fogType = glGetUniformLocation(blinnPhongShaderProgram, "fog_Type");
		blinnPhongShaderUniforms.fogColor = glGetUniformLocation(blinnPhongShaderProgram, "fogColor");
		blinnPhongShaderUniforms.fogDensity = glGetUniformLocation(blinnPhongShaderProgram, "fogDensity");
		blinnPhongShaderUniforms.bi_be_factor = glGetUniformLocation(blinnPhongShaderProgram, "bi_be_factor");
		blinnPhongShaderUniforms.fogStart = glGetUniformLocation(blinnPhongShaderProgram, "fog_start");
		blinnPhongShaderUniforms.fogEnd = glGetUniformLocation(blinnPhongShaderProgram, "fog_end");

		glUniform1i(glGetUniformLocation(blinnPhongShaderProgram, "diffuseTexture"), 0);
		glUniform1i(glGetUniformLocation(blinnPhongShaderProgram, "shadowMap_directional"), 1);
		glUniform1i(glGetUniformLocation(blinnPhongShaderProgram, "shadowMap_point"), 2);
	}

	directionalLightDepthShaderProgram = loadShader(dirPath + "directionalLight_depth.vs", dirPath + "directionalLight_depth.fs");
	{
		glUseProgram(directionalLightDepthShaderProgram);

		directionalLightDepthShaderUniforms.model_matrix = glGetUniformLocation(directionalLightDepthShaderProgram, "model");
		directionalLightDepthShaderUniforms.light_matrix = glGetUniformLocation(directionalLightDepthShaderProgram, "lightSpaceMatrix");

		glUniform1i(glGetUniformLocation(directionalLightDepthShaderProgram, "diffuseTexture"), 0);
	}

	pointLightDepthShaderProgram = loadShader(dirPath + "pointLight_depth.vs", dirPath + "pointLight_depth.fs", dirPath + "pointLight_depth.gs");
	{
		glUseProgram(pointLightDepthShaderProgram);

		pointLightDepthShaderUniforms.model_matrix = glGetUniformLocation(pointLightDepthShaderProgram, "model");
		pointLightDepthShaderUniforms.lightPos = glGetUniformLocation(pointLightDepthShaderProgram, "lightPos");
		pointLightDepthShaderUniforms.farPlane = glGetUniformLocation(pointLightDepthShaderProgram, "farPlane");
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	m_screenSize = vec2(800, 800);
	m_camera.SetWindowSize(800, 800);
	m_camera.SetZoom(3.0f);
	m_camera.SetRotation(deg2rad(90), 0);
	m_camera.Translate(vec3(0, 200.0, -0.6));

	setupShaders();
	setupModels();

	glUseProgram(blinnPhongShaderProgram);
	glUniform1i(blinnPhongShaderUniforms.pcfKernel, 1);
	glUseProgram(0);

	// configure depth map FBO
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
	//glClearColor(0.486, 0.721, 0.918, 1.000); // 124, 184, 234
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
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

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glUseProgram(0);
	}
	else if (m_lightSource_type == 1) // point light
	{
		// create depth cubemap transformation matrices (shadow matrices)
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

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glUseProgram(0);
	}

	// reset viewport
	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	glUseProgram(blinnPhongShaderProgram);
	{
		glUniform1i(blinnPhongShaderUniforms.lightSourceType, m_lightSource_type);
		glUniform1f(blinnPhongShaderUniforms.farPlane, POINT_LIGHT_FARPLANE);
		glUniform3fv(blinnPhongShaderUniforms.lightPos, 1, value_ptr(m_lightPos));
		glUniform3fv(blinnPhongShaderUniforms.viewPos, 1, value_ptr(m_camera.GetWorldEyePosition()));
		glUniformMatrix4fv(blinnPhongShaderUniforms.light_matrix, 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(blinnPhongShaderUniforms.view_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
		glUniformMatrix4fv(blinnPhongShaderUniforms.projection_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));

		glUniform1i(blinnPhongShaderUniforms.fogType, fogType);
		glUniform1f(blinnPhongShaderUniforms.fogDensity, fogDensity);
		fogEnd = fogEnd < fogStart ? fogStart : fogEnd;
		glUniform1f(blinnPhongShaderUniforms.fogStart, fogStart);
		glUniform1f(blinnPhongShaderUniforms.fogEnd, fogEnd);
		glUniform2f(blinnPhongShaderUniforms.bi_be_factor, bi_be_factor.x, bi_be_factor.y);
		glUniform4f(blinnPhongShaderUniforms.fogColor, fogColor.x, fogColor.y, fogColor.z, fogColor.w);

		for (int i = 0; i < m_shapes.size(); ++i)
		{
			Shape m_shape = m_shapes[i];

			glBindVertexArray(m_shape.vao);
			glUniformMatrix4fv(blinnPhongShaderUniforms.model_matrix, 1, GL_FALSE, value_ptr(m_shape.getTransformationMatrix()));
			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
			glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
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
		if(m_lightSource_type == 0)
			m_lightPos = glm::rotateY(m_lightPos, 1.0f * 0.01f);
		else
			m_lightPos = glm::rotateY(m_lightPos, 1.0f * 0.02f);
	}

	checkLightSourceTypeChanged();

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
			takeScreenshotPNG("./frame.png");
		}
		else if (key == 'p' || key == 'P')
		{

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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("13-2. Fog"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
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