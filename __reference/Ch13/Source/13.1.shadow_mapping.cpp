#include "../Include/AntTweakBar/AntTweakBar.h"
#include "../Include/Common.h"
#include "../Include/ViewManager.h"
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

struct
{
	GLint  near_plane;
	GLint  far_plane;
	GLint  lightSourceType;
	GLint  screenSize;
} shadowQuadShaderUniforms;

// Shader programs

GLuint			blinnPhongShadowShaderProgram;
GLuint			directionalLightDepthShaderProgram;
GLuint			pointLightDepthShaderProgram;
GLuint			shadowQuadShaderProgram;


// Global variables

vector<Shape>	m_shapes;
ViewManager		m_camera;
mat4			proj_matrix;
float			aspect;
vec3			m_lightPos = vec3(25, 25, 0);
GLuint			m_directionalLight_depthMapFBO;
GLuint			m_pointLight_depthMapFBO;
GLuint			m_directionalLight_shadowDepthMap;
GLuint			m_pointLight_shadowDepthMap;
vec2			m_screenSize;
GLuint			m_shadowQuadVAO = 0;
TwBar			*bar;


// GUI control variables

int				m_lightSource_type = 0;
int				m_last_lightSource_type = 1;
int				m_shadowmap_resolution = 2048;
int				m_last_shadowmap_resolution = m_shadowmap_resolution;
int				m_useShadowmap = 1;
int				m_autoRotate = 1;
int				m_showShadowDepthMap = 0;
int				m_useBias = 1;
int				m_pcfKernel = 1;
int				m_last_pcfKernel = 1;
float			m_lightRotateSpeed = 1.0;

// GUI callbacks

void TW_CALL SetEnableShadowmapCB(const void *value, void *clientData)
{
	m_useShadowmap = *(const int *)value; // copy value to g_AutoRotate
	
	glUseProgram(blinnPhongShadowShaderProgram);
	glUniform1i(blinnPhongShaderUniforms.useShadowmap, (m_useShadowmap > 0) ? 1 : 0);
	glUseProgram(0);
}
void TW_CALL GetEnableShadowmapCB(void *value, void *clientData)
{
	*(int *)value = m_useShadowmap;
}
void TW_CALL SetAutoRotateCB(const void *value, void *clientData)
{
	m_autoRotate = *(const int *)value;
}
void TW_CALL GetAutoRotateCB(void *value, void *clientData)
{
	*(int *)value = m_autoRotate;
}
void TW_CALL SetShowShadowDepthMapCB(const void *value, void *clientData)
{
	m_showShadowDepthMap = *(const int *)value;
}
void TW_CALL GetShowShadowDepthMapCB(void *value, void *clientData)
{
	*(int *)value = m_showShadowDepthMap;
}
void TW_CALL SetUseBiasCB(const void *value, void *clientData)
{
	m_useBias = *(const int *)value;

	glUseProgram(blinnPhongShadowShaderProgram);
	glUniform1i(blinnPhongShaderUniforms.useBias, (m_useBias > 0) ? 1 : 0);
	glUseProgram(0);
}
void TW_CALL GetUseBiasCB(void *value, void *clientData)
{
	*(int *)value = m_useBias;
}

// Helpers

void checkShadowMapResolutionChanged()
{
	if (m_last_shadowmap_resolution != m_shadowmap_resolution)
	{
		m_shadowmap_resolution = m_last_shadowmap_resolution;

		// directional light depthmap
		{
			glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
			glDeleteTextures(1, &m_directionalLight_shadowDepthMap);

			glGenTextures(1, &m_directionalLight_shadowDepthMap);
			glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowmap_resolution, m_shadowmap_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glBindFramebuffer(GL_FRAMEBUFFER, m_directionalLight_depthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_directionalLight_shadowDepthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// point light depthmap
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
			glDeleteTextures(1, &m_pointLight_shadowDepthMap);

			glGenTextures(1, &m_pointLight_shadowDepthMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
			for (int i = 0; i < 6; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_shadowmap_resolution, m_shadowmap_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindFramebuffer(GL_FRAMEBUFFER, m_pointLight_depthMapFBO);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_pointLight_shadowDepthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	}
}
void checkPcfKernelChanged()
{
	if (m_last_pcfKernel != m_pcfKernel)
	{
		m_pcfKernel = m_last_pcfKernel;

		glUseProgram(blinnPhongShadowShaderProgram);
		glUniform1i(blinnPhongShaderUniforms.pcfKernel, m_pcfKernel);
		glUseProgram(0);
	}
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
			m_lightPos = vec3(5, 4, 0);
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
	char** vsSource = Common::LoadShaderSource(vsPath.c_str());
	char** fsSource = Common::LoadShaderSource(fsPath.c_str());
	char** gsSource = (useGeometricShader) ? Common::LoadShaderSource(gsPath.c_str()) : NULL;
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	if(useGeometricShader) glShaderSource(gs, 1, gsSource, NULL);
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

// Initialization functions

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

	TwAddVarCB(bar, "Enable Shadow mapping", TW_TYPE_BOOL32, SetEnableShadowmapCB, GetEnableShadowmapCB, NULL, "");
	TwAddVarCB(bar, "Auto Rotate", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL, "");
	TwAddVarCB(bar, "Bias Correction", TW_TYPE_BOOL32, SetUseBiasCB, GetUseBiasCB, NULL, "");
	TwAddVarCB(bar, "Show Shadow Depth Map", TW_TYPE_BOOL32, SetShowShadowDepthMapCB, GetShowShadowDepthMapCB, NULL, "");
	{
		TwEnumVal resolutionEV[7] = { { 64, "64 x 64" },{ 128, "128 x 128" },{ 256, "256 x 256" },{ 512, "512 x 512" },{ 1024, "1024 x 1024" },{ 2048, "2048 x 2048" },{ 4096, "4096 x 4096" } };
		TwType type = TwDefineEnum("resolution", resolutionEV, 7);
		TwAddVarRW(bar, "Shadow map resolution", type, &m_last_shadowmap_resolution, "");
	}
	{
		TwEnumVal kernelEV[5] = { { 0, "None" },{ 1, "3 x 3" },{ 4, "9 x 9" },{ 7, "15 x 15" },{ 10, "21 x 21" } };
		TwType type = TwDefineEnum("kernelSize", kernelEV, 5);
		TwAddVarRW(bar, "PCF Kernel Size", type, &m_last_pcfKernel, "");
	}
	{
		TwEnumVal lightSourceTypeEV[2] = { { 0, "Directional Light" },{ 1, "Point Light" } };
		TwType type = TwDefineEnum("LightSourceType", lightSourceTypeEV, 2);
		TwAddVarRW(bar, "Light source type", type, &m_last_lightSource_type, "");
	}
}
void setupModels()
{
	int numModels = 9;
	string model_names[] = 
	{
		"ladybug.obj",
		"ladybug.obj",
		"ladybug.obj",
		"ground.obj",
		"ground.obj",
		"ground.obj",
		"ground.obj",
		"ground.obj",
		"tree.obj",
	};

	int numTextures = 10;
	string texture_names[] = 
	{
		"ladybug_diff.png",
		"ladybug_diff.png",
		"ladybug_diff.png",
		"wood.png",
		"wood.png",
		"wood.png",
		"wood.png",
		"wood.png",
		"shugan.png",
		"yezi.png"
	};

	map<string, vector<tinyobj::shape_t>> model_cache;
	map<string, TextureData> texture_cache;

	for(int i = 0; i < numModels; i++)
	{
		std::vector<tinyobj::shape_t> shapes;
		map<string, vector<tinyobj::shape_t>>::iterator found = model_cache.find(model_names[i]);
		if (found != model_cache.end())
		{
			shapes = found->second;
		}
		else
		{		
			std::vector<tinyobj::material_t> materials;
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

			printf("Add object %s\n", shapes[j].name.c_str());
			m_shapes.push_back(m_shape);
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_shapes[0].position = vec3(5, 0, -5);
	m_shapes[1].position = vec3(-5, 0, -5);
	m_shapes[1].rotation = vec3(0, 45, 0);
	m_shapes[1].scale = vec3(0.5);
	m_shapes[2].position = vec3(6, 0, 1);
	m_shapes[2].rotation = vec3(0, -30, 0);
	m_shapes[2].scale = vec3(0.2);
	m_shapes[4].position = vec3(0, 0, 10);
	m_shapes[4].rotation = vec3(-90, 0, 0);
	m_shapes[5].position = vec3(0, 0, -10);
	m_shapes[5].rotation = vec3(90, 0, 0);
	m_shapes[6].position = vec3(10, 0, 0);
	m_shapes[6].rotation = vec3(0, 0, -90);
	m_shapes[7].position = vec3(-10, 0, 0);
	m_shapes[7].rotation = vec3(0, 0, 90);
}
void setupShaders()
{
	string dirPath = "13.1.shadow_mapping/";

	blinnPhongShadowShaderProgram = loadShader(dirPath + "blinnPhong_shadow.vs", dirPath + "blinnPhong_shadow.fs");
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

		glUniform1i(glGetUniformLocation(blinnPhongShadowShaderProgram, "diffuseTexture"), 0);
		glUniform1i(glGetUniformLocation(blinnPhongShadowShaderProgram, "shadowMap_directional"), 1);
		glUniform1i(glGetUniformLocation(blinnPhongShadowShaderProgram, "shadowMap_point"), 2);
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

	shadowQuadShaderProgram = loadShader(dirPath + "shadowQuad.vs", dirPath + "shadowQuad.fs");
	{
		glUseProgram(shadowQuadShaderProgram);

		shadowQuadShaderUniforms.near_plane= glGetUniformLocation(shadowQuadShaderProgram, "near_plane");
		shadowQuadShaderUniforms.far_plane = glGetUniformLocation(shadowQuadShaderProgram, "far_plane");
		shadowQuadShaderUniforms.lightSourceType = glGetUniformLocation(shadowQuadShaderProgram, "lightSourceType");
		shadowQuadShaderUniforms.screenSize = glGetUniformLocation(shadowQuadShaderProgram, "screenSize");

		glUniform1i(glGetUniformLocation(shadowQuadShaderProgram, "shadowMap_directional"), 0);
		glUniform1i(glGetUniformLocation(shadowQuadShaderProgram, "shadowMap_point"), 1);
	}
}

// GLUT callbacks

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
	m_camera.SetRotation(0, deg2rad(30));
	m_camera.Translate(vec3(0, 0, -0.5));

	setupShaders();
	setupModels();

	glUseProgram(blinnPhongShadowShaderProgram);
	glUniform1i(blinnPhongShaderUniforms.useShadowmap, (m_useShadowmap > 0) ? 1 : 0);
	glUniform1i(blinnPhongShaderUniforms.useBias, (m_useBias > 0) ? 1 : 0);
	glUniform1i(blinnPhongShaderUniforms.pcfKernel, m_pcfKernel);
	glUseProgram(0);

	// configure depth map FBO for directional light
	// -----------------------
	glGenFramebuffers(1, &m_directionalLight_depthMapFBO);
	// create depth texture
	glGenTextures(1, &m_directionalLight_shadowDepthMap);
	glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowmap_resolution, m_shadowmap_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_shadowmap_resolution, m_shadowmap_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
	glClearColor(0.486, 0.721, 0.918, 1.000); // 124, 184, 234
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//printf("%f %f %f\n", m_lightPos[0], m_lightPos[1], m_lightPos[2]);

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
			glViewport(0, 0, m_shadowmap_resolution, m_shadowmap_resolution);
			glBindFramebuffer(GL_FRAMEBUFFER, m_directionalLight_depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < m_shapes.size(); ++i)
			{
				if(m_lightSource_type == 0 && i >= 4 && i <= 7) continue;

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
		mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_shadowmap_resolution / (float)m_shadowmap_resolution, POINT_LIGHT_NEARPLANE, POINT_LIGHT_FARPLANE);
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

			glViewport(0, 0, m_shadowmap_resolution, m_shadowmap_resolution);
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

	if (m_showShadowDepthMap == 0)
	{
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

			for (int i = 0; i < m_shapes.size(); ++i)
			{
				if (m_lightSource_type == 0 && i >= 4 && i <= 7) continue;

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
	else
	{		
		glUseProgram(shadowQuadShaderProgram);
		{
			if (m_shadowQuadVAO == 0)
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
				GLuint m_shadowQuadVBO;
				glGenVertexArrays(1, &m_shadowQuadVAO);
				glGenBuffers(1, &m_shadowQuadVBO);
				glBindVertexArray(m_shadowQuadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, m_shadowQuadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				printf("shadowQuadVAO: %d\n", m_shadowQuadVAO);
			}
			glUniform1f(shadowQuadShaderUniforms.near_plane, DIRECTIONAL_LIGHT_NEARPLANE);
			glUniform1f(shadowQuadShaderUniforms.far_plane, DIRECTIONAL_LIGHT_FARPLANE);
			glUniform1i(shadowQuadShaderUniforms.lightSourceType, m_lightSource_type);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
			glBindVertexArray(m_shadowQuadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}
		glUseProgram(0);
	}
	/////////////////////////////////////////////////////////////////////////////////
	TwDraw();
    glutSwapBuffers();
	glutPostRedisplay();
}
void My_Reshape(int width, int height)
{
	m_screenSize = vec2(width, height);

	glUseProgram(shadowQuadShaderProgram);
	glUniform2fv(shadowQuadShaderUniforms.screenSize, 1, value_ptr(m_screenSize));
	glUseProgram(0);

	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);

	TwWindowSize(width, height);
}
void My_Timer(int val)
{
	//GLfloat move = glutGet(GLUT_ELAPSED_TIME) / 50.0;

	if (m_autoRotate)
		m_lightPos = glm::rotateY(m_lightPos, m_lightRotateSpeed * 0.01f);

	checkShadowMapResolutionChanged();
	checkPcfKernelChanged();
	checkLightSourceTypeChanged();

	glutPostRedisplay();
	glutTimerFunc(16, My_Timer, val);
}
void My_Mouse(int button, int state, int x, int y)
{
	if (!TwEventMouseButtonGLUT(button, state, x, y))
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
		else if (key == 'y' || key == 'Y')
		{
// 			string sideStr[6] = { "posX", "negX","posY", "negY","posZ", "negZ", };
// 			GLenum targets[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
// 								  GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,
// 								  GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
// 
// 			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
// 			for (int side = 0; side < 6; ++side)
// 			{
// 				int size = m_shadowmap_resolution * m_shadowmap_resolution;
// 				GLfloat *pixels = new GLfloat[size];
// 				unsigned char *outputPixels = new unsigned char[size * 3];
// 				glGetTexImage(targets[side], 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
// 				for (int i = 0; i < size; ++i)
// 				{
// 					if (pixels[i] < 0 || pixels[i] > 1)
// 					{
// 						cout << pixels[i] << endl;
// 					}
// 
// 					outputPixels[i * 3] = pixels[i] * 255;
// 					outputPixels[i * 3 + 1] = pixels[i] * 255;
// 					outputPixels[i * 3 + 2] = pixels[i] * 255;
// 				}
// 				stbi_write_png(string("./" + sideStr[side] + ".png").c_str(), m_shadowmap_resolution, m_shadowmap_resolution, 3, outputPixels, 0);
// 				delete pixels, outputPixels;
//			}
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
	glutCreateWindow("13-1. Shadow"); 
#ifdef _MSC_VER
	glewInit();
#endif

	Common::DumpInfo();

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
