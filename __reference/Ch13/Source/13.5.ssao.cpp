#include "../Include/AntTweakBar/AntTweakBar.h"
#include "../Include/Common.h"
#include "../Include/viewmanager.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Include/STB/stb_image_write.h"
#include <random>

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


#define NUM_LIGHTS			64
#define CAMERA_NEAR_PLANE	0.1f
#define CAMERA_FAR_PLANE	150.0f
#define NUM_SSAO_KERNELS	64

struct
{
	GLint  gBuffer_type;
	GLint  near_plane;
	GLint  far_plane;
} gBufferVisualizationShaderUniforms;

struct
{
	GLint  model_matrix;
	GLint  view_matrix;
	GLint  projection_matrix;
} gBufferShaderUniforms;

struct
{
	GLint lightPositions[NUM_LIGHTS];
	GLint lightColors[NUM_LIGHTS];
	GLint useSSAO;
} ssaoDeferredShadingShaderUniforms;

struct
{
	GLint sampleKernels[NUM_SSAO_KERNELS];
	GLint projection_matrix;
	GLint noiseScale;
	GLint radius;
	GLint bias;
} ssaoShaderUniforms;

typedef struct G_Buffer
{
	GLuint position;
	GLuint normal;
	GLuint diffuseColor;
	GLuint depth;
};

GLuint			gBufferShaderProgram;
GLuint			gBufferVisualizationShaderProgram;
GLuint			ssaoDeferredShadingShaderProgram;
GLuint			ssaoShaderProgram;
GLuint			ssaoBlurShaderProgram;

mat4			proj_matrix;
float			aspect;

vector<Shape>	m_shapes;
ViewManager		m_camera;
vec3			m_lightPos = vec3(25, 25, 0);
GLuint			m_gBufferFBO;
GLuint			m_ssaoFBO;
GLuint			m_ssaoBlurFBO;
GLuint			m_ssaoTexID;
GLuint			m_ssaoBlurTexID;
GLuint			m_ssaoNoiseTexID;
GLuint			m_quadVAO;
int				m_G_Buffer_Type = 0;
int				m_last_G_Buffer_Type = m_G_Buffer_Type;
vec2			m_screenSize;
int				m_useShadowmap = 1;
int				m_autoRotate = 1;
int				m_showShadowDepthMap = 0;
int				m_useBias = 1;
int				m_pcfKernel = 1;
int				m_last_pcfKernel = 1;
int				m_useSSAO = 1;
int				m_enableBlurringSSAO = 1;
int				m_showLights = 1;
float			m_lightRotateSpeed = 1.0;
float			m_ssao_radius = 0.5;
float			m_ssao_bias = 0.025;
TwBar			*bar;
G_Buffer		m_gBuffer;
vector<vec3>	m_lightPositions;
vector<vec3>	m_lightColors;
vector<vec3>	m_ssaoSampleKernels;

void TW_CALL SetUseSSAOCB(const void *value, void *clientData)
{
	m_useSSAO = *(const int *)value;

	glUseProgram(ssaoDeferredShadingShaderProgram);
	glUniform1i(ssaoDeferredShadingShaderUniforms.useSSAO, (m_useSSAO > 0) ? 1 : 0);
	glUseProgram(0);
}
void TW_CALL GetUseSSAOCB(void *value, void *clientData)
{
	*(int *)value = m_useSSAO;
}
void TW_CALL SetSSAO_RadiusCB(const void *value, void *clientData)
{
	m_ssao_radius = *(const float *)value;

	glUseProgram(ssaoShaderProgram);
	glUniform1f(ssaoShaderUniforms.radius, m_ssao_radius);
	glUseProgram(0);
}
void TW_CALL GetSSAO_RadiusCB(void *value, void *clientData)
{
	*(float *)value = m_ssao_radius;
}
void TW_CALL SetSSAO_BiasCB(const void *value, void *clientData)
{
	m_ssao_bias = *(const float *)value;

	glUseProgram(ssaoShaderProgram);
	glUniform1f(ssaoShaderUniforms.bias, m_ssao_bias);
	glUseProgram(0);
}
void TW_CALL GetSSAO_BiasCB(void *value, void *clientData)
{
	*(float *)value = m_ssao_bias;
}
void TW_CALL SetEnableBlurringSSAOCB(const void *value, void *clientData)
{
	m_enableBlurringSSAO = *(const int *)value;
}
void TW_CALL GetEnableBlurringSSAOCB(void *value, void *clientData)
{
	*(int *)value = m_enableBlurringSSAO;
}
void TW_CALL SetShowLightsSSAOCB(const void *value, void *clientData)
{
	m_showLights = *(const int *)value;
}
void TW_CALL GetShowLightsSSAOCB(void *value, void *clientData)
{
	*(int *)value = m_showLights;
}

GLuint loadShader(string vsPath, string fsPath)
{
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = Common::LoadShaderSource(vsPath.c_str());
	char** fsSource = Common::LoadShaderSource(fsPath.c_str());
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	Common::FreeShaderSource(vsSource);
	Common::FreeShaderSource(fsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	Common::ShaderLog(vs);
	Common::ShaderLog(fs);
	//Attach Shader to program
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	return shaderProgram;
}
void resizeBuffers(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFBO);
	{
		glDeleteTextures(1, &m_gBuffer.position);
		glDeleteTextures(1, &m_gBuffer.normal);
		glDeleteTextures(1, &m_gBuffer.diffuseColor);
		glDeleteTextures(1, &m_gBuffer.depth);

		// - Position color buffer
		glGenTextures(1, &m_gBuffer.position);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.position);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBuffer.position, 0);

		// - Normal color buffer
		glGenTextures(1, &m_gBuffer.normal);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.normal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBuffer.normal, 0);

		// - Color + Specular color buffer
		glGenTextures(1, &m_gBuffer.diffuseColor);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.diffuseColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBuffer.diffuseColor, 0);

		glGenTextures(1, &m_gBuffer.depth);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_gBuffer.depth, 0);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	{
		glDeleteTextures(1, &m_ssaoTexID);

		glGenTextures(1, &m_ssaoTexID);
		glBindTexture(GL_TEXTURE_2D, m_ssaoTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoTexID, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	{
		glDeleteTextures(1, &m_ssaoBlurTexID);

		glGenTextures(1, &m_ssaoBlurTexID);
		glBindTexture(GL_TEXTURE_2D, m_ssaoBlurTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurTexID, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void checkGbufferTypeChanged()
{
	if (m_last_G_Buffer_Type != m_G_Buffer_Type)
	{
		m_G_Buffer_Type = m_last_G_Buffer_Type;

		glUseProgram(gBufferVisualizationShaderProgram);
		glUniform1i(gBufferVisualizationShaderUniforms.gBuffer_type, m_G_Buffer_Type);
		glUseProgram(0);
	}
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
	TwDefine("Properties fontsize='3' size='260 150' color='0 0 0' alpha=128 valueswidth=fit");  // http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:twbarparamsyntax

	TwAddVarCB(bar, "Show Lights", TW_TYPE_BOOL32, SetShowLightsSSAOCB, GetShowLightsSSAOCB, NULL, "");
	TwAddVarCB(bar, "Enable SSAO", TW_TYPE_BOOL32, SetUseSSAOCB, GetUseSSAOCB, NULL, "");
	TwAddVarCB(bar, "Enable Blurring SSAO", TW_TYPE_BOOL32, SetEnableBlurringSSAOCB, GetEnableBlurringSSAOCB, NULL, "");
	TwAddVarCB(bar, "SSAO Radius", TW_TYPE_FLOAT, SetSSAO_RadiusCB, GetSSAO_RadiusCB, NULL, "");
	TwAddVarCB(bar, "SSAO Bias", TW_TYPE_FLOAT, SetSSAO_BiasCB, GetSSAO_BiasCB, NULL, "");
	{
		TwEnumVal gBufferTypeEV[7] = { { 0, "Combined" },{ 1, "Position" },{ 2, "Normal" },{ 3, "Diffuse Color" },{ 4, "Depth" },{ 5, "SSAO" },{ 6, "SSAO Blur" } };
		TwType type = TwDefineEnum("bufferType", gBufferTypeEV, 7);
		TwAddVarRW(bar, "G-Buffer Type", type, &m_last_G_Buffer_Type, "");
	}
}
void setupModels()
{
	bool enableAutoTextureDetection = true;
	string material_basePath = "dabrovic-sponza/";

	int numModels = 1;
	string model_names[] =
	{
		"dabrovic-sponza/sponza.obj"
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

			bool ret = tinyobj::LoadObj(shapes, materials, err, model_names[i].c_str(), material_basePath.c_str());
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

			if (enableAutoTextureDetection)
			{
				string textureFileName;
				if (shapes[j].mesh.material_ids.size() > 0)
				{
					textureFileName = materials[shapes[j].mesh.material_ids[0]].diffuse_texname.c_str();
				}
				else
				{
					textureFileName = "";
				}
					
				TextureData textureData;
				map<string, TextureData>::iterator found = texture_cache.find(textureFileName);
				if (found != texture_cache.end())
				{
					textureData = found->second;
				}
				else
				{
					textureData = Common::Load_png((material_basePath + textureFileName).c_str());
					texture_cache.insert(pair<string, TextureData>(textureFileName, textureData));
				}
				glGenTextures(1, &m_shape.m_texture);
				glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			m_shapes.push_back(m_shape);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//for (int i = 0; i < m_shapes.size(); ++i)
	//{
	//	m_shapes[i].scale = vec3(0.025);
	//}
}
void setupShaders()
{
	string dirPath = "13.5.ssao/";

	gBufferShaderProgram = loadShader(dirPath + "SSAO-G-Buffer.vs", dirPath + "SSAO-G-Buffer.fs");
	{
		glUseProgram(gBufferShaderProgram);

		//Cache uniform variable id
		gBufferShaderUniforms.model_matrix = glGetUniformLocation(gBufferShaderProgram, "model");
		gBufferShaderUniforms.view_matrix = glGetUniformLocation(gBufferShaderProgram, "view");
		gBufferShaderUniforms.projection_matrix = glGetUniformLocation(gBufferShaderProgram, "projection");

		glUniform1i(glGetUniformLocation(gBufferShaderProgram, "diffuseTexture"), 0);
	}

	gBufferVisualizationShaderProgram = loadShader(dirPath + "SSAO-G-Buffer-Visualization.vs", dirPath + "SSAO-G-Buffer-Visualization.fs");
	{
		glUseProgram(gBufferVisualizationShaderProgram);

		//Cache uniform variable id		
		gBufferVisualizationShaderUniforms.gBuffer_type = glGetUniformLocation(gBufferVisualizationShaderProgram, "gBuffer_type");
		gBufferVisualizationShaderUniforms.near_plane = glGetUniformLocation(gBufferVisualizationShaderProgram, "near_plane");
		gBufferVisualizationShaderUniforms.far_plane = glGetUniformLocation(gBufferVisualizationShaderProgram, "far_plane");

		glUniform1i(glGetUniformLocation(gBufferVisualizationShaderProgram, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(gBufferVisualizationShaderProgram, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(gBufferVisualizationShaderProgram, "gDiffuse"), 2);
		glUniform1i(glGetUniformLocation(gBufferVisualizationShaderProgram, "gDepth"), 3);
		glUniform1i(glGetUniformLocation(gBufferVisualizationShaderProgram, "gSSAO"), 4);
		glUniform1i(glGetUniformLocation(gBufferVisualizationShaderProgram, "gSSAO_blur"), 5);
	}

	ssaoDeferredShadingShaderProgram = loadShader(dirPath + "SSAO.vs", dirPath + "SSAO-Deferred-Shading.fs");
	{
		glUseProgram(ssaoDeferredShadingShaderProgram);

		//Cache uniform variable id
		ssaoDeferredShadingShaderUniforms.useSSAO = glGetUniformLocation(ssaoDeferredShadingShaderProgram, "useSSAO");
		for (int i = 0; i < NUM_LIGHTS; ++i)
		{
			ssaoDeferredShadingShaderUniforms.lightPositions[i] = glGetUniformLocation(ssaoDeferredShadingShaderProgram, string("lights[" + to_string(i) + "].Position").c_str());
			ssaoDeferredShadingShaderUniforms.lightColors[i] = glGetUniformLocation(ssaoDeferredShadingShaderProgram, string("lights[" + to_string(i) + "].Color").c_str());
		}

		glUniform1i(glGetUniformLocation(ssaoDeferredShadingShaderProgram, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(ssaoDeferredShadingShaderProgram, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(ssaoDeferredShadingShaderProgram, "gDiffuse"), 2);
		glUniform1i(glGetUniformLocation(ssaoDeferredShadingShaderProgram, "ssao"), 3);
	}

	ssaoShaderProgram = loadShader(dirPath + "SSAO.vs", dirPath + "SSAO.fs");
	{
		glUseProgram(ssaoShaderProgram);

		ssaoShaderUniforms.projection_matrix = glGetUniformLocation(ssaoShaderProgram, "projection");
		ssaoShaderUniforms.noiseScale = glGetUniformLocation(ssaoShaderProgram, "noiseScale");
		ssaoShaderUniforms.radius = glGetUniformLocation(ssaoShaderProgram, "radius");
		ssaoShaderUniforms.bias = glGetUniformLocation(ssaoShaderProgram, "bias");
		for (int i = 0; i < NUM_SSAO_KERNELS; ++i)
		{
			ssaoShaderUniforms.sampleKernels[i] = glGetUniformLocation(ssaoShaderProgram, string("samples[" + to_string(i) + "]").c_str());
		}

		glUniform1i(glGetUniformLocation(ssaoShaderProgram, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(ssaoShaderProgram, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(ssaoShaderProgram, "texNoise"), 2);
	}

	ssaoBlurShaderProgram = loadShader(dirPath + "SSAO.vs", dirPath + "SSAO-Blur.fs");
	{
		glUseProgram(ssaoBlurShaderProgram);

		glUniform1i(glGetUniformLocation(ssaoBlurShaderProgram, "ssaoInput"), 0);
	}
}
void setupBuffers()
{
	// G-Buffer FBO
	glGenFramebuffers(1, &m_gBufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFBO);
	{
		// - Position color buffer
		glGenTextures(1, &m_gBuffer.position);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.position);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBuffer.position, 0);

		// - Normal color buffer
		glGenTextures(1, &m_gBuffer.normal);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.normal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBuffer.normal, 0);

		// - Color + Specular color buffer
		glGenTextures(1, &m_gBuffer.diffuseColor);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.diffuseColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screenSize.x, m_screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBuffer.diffuseColor, 0);

		glGenTextures(1, &m_gBuffer.depth);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_gBuffer.depth, 0);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// SSAO FBO
	glGenFramebuffers(1, &m_ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	{
		glGenTextures(1, &m_ssaoTexID);
		glBindTexture(GL_TEXTURE_2D, m_ssaoTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoTexID, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// SSAO Blur FBO
	glGenFramebuffers(1, &m_ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	{
		glGenTextures(1, &m_ssaoBlurTexID);
		glBindTexture(GL_TEXTURE_2D, m_ssaoBlurTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurTexID, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void My_Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);

	m_screenSize = vec2(800, 800);
	m_camera.SetWindowSize(800, 800);
	m_camera.SetZoom(3.0f);
	m_camera.SetRotation(deg2rad(90), 0);
	m_camera.Translate(vec3(0, 200.0, -0.6));

	m_lightPositions.resize(NUM_LIGHTS);
	m_lightColors.resize(NUM_LIGHTS);
	for (int i = 0; i < m_lightPositions.size(); ++i)
	{
		float xPos = ((rand() % 100) / 100.0) * 20.0 - 10.0;
		float yPos = ((rand() % 100) / 100.0) * 10.0 + 1.0;
		float zPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
		float rColor = ((rand() % 100) / 200.0f) + 0.5;
		float gColor = ((rand() % 100) / 200.0f) + 0.5;
		float bColor = ((rand() % 100) / 200.0f) + 0.5;

		m_lightPositions[i] = vec3(xPos, yPos, zPos);
		m_lightColors[i] = vec3(rColor, gColor, bColor);
	}

	setupShaders();
	setupModels();
	setupBuffers();

	// Shader initialization
	glUseProgram(ssaoShaderProgram);
	glUniform2fv(ssaoShaderUniforms.noiseScale, 1, value_ptr(m_screenSize / 4.0f));
	glUniform1f(ssaoShaderUniforms.bias, m_ssao_bias);
	glUniform1f(ssaoShaderUniforms.radius, m_ssao_radius);
	glUseProgram(0);

	glUseProgram(ssaoDeferredShadingShaderProgram);
	glUniform1i(ssaoDeferredShadingShaderUniforms.useSSAO, (m_useSSAO > 0) ? 1 : 0);
	glUseProgram(0);

	// Generate SSAO sample kernels
	uniform_real_distribution<float> randomFloats(0.0, 1.0);
	default_random_engine generator;

	m_ssaoSampleKernels.resize(NUM_SSAO_KERNELS);
	for (int i = 0; i < NUM_SSAO_KERNELS; ++i)
	{
		vec3 sample
		(
			randomFloats(generator) * 2.0f - 1.0f, 
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator)
		);

		sample = glm::normalize(sample);
		sample *= randomFloats(generator); // random length scaling [0.0, 1.0]

		float weight = float(i) / NUM_SSAO_KERNELS;
		weight = 0.1f + (weight * weight) * (1.0f - 0.1f); // lerp from 0.1 to 1.0
		sample *= weight;

		m_ssaoSampleKernels[i] = sample;
	}

	// Generate SSAO noise texture
	vector<vec3> noises;
	for (unsigned int i = 0; i < 16; i++)
	{
		// rotate around z-axis (in tangent space)
		vec3 noise
		(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f
		);
		noises.push_back(noise);
	}
	glGenTextures(1, &m_ssaoNoiseTexID);
	glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &noises[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void My_Display()
{
	glClearColor(0.486, 0.721, 0.918, 1.000); // 124, 184, 234
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. geometry pass: render scene's geometry/color data into G-buffer
	// -----------------------------------------------------------------
	glUseProgram(gBufferShaderProgram);
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(gBufferShaderUniforms.view_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
		glUniformMatrix4fv(gBufferShaderUniforms.projection_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));

		for (int i = 0; i < m_shapes.size(); ++i)
		{
			Shape m_shape = m_shapes[i];

			glBindVertexArray(m_shape.vao);
			glUniformMatrix4fv(gBufferShaderUniforms.model_matrix, 1, GL_FALSE, value_ptr(m_shape.getTransformationMatrix()));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_shape.m_texture);
			glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glUseProgram(0);

	// 2. generate SSAO texture
	// ------------------------
	glUseProgram(ssaoShaderProgram);
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		glUniformMatrix4fv(ssaoShaderUniforms.projection_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
		for (int i = 0; i < NUM_SSAO_KERNELS; ++i)
		{
			glUniform3fv(ssaoShaderUniforms.sampleKernels[i], 1, value_ptr(m_ssaoSampleKernels[i]));
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.position);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexID);
		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glUseProgram(0);

	// 3. blur SSAO texture to remove noise
	// ------------------------------------
	glUseProgram(ssaoBlurShaderProgram);
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ssaoTexID);
		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glUseProgram(0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_G_Buffer_Type == 0)
	{
		// 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
		// -----------------------------------------------------------------------------------------------------
		glUseProgram(ssaoDeferredShadingShaderProgram);
		{
			for (int i = 0; i < m_lightPositions.size(); ++i)
			{
				vec4 viewSpaceLightPosition = vec4(m_lightPositions[i], 1);
				viewSpaceLightPosition = m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * viewSpaceLightPosition;

				glUniform3fv(ssaoDeferredShadingShaderUniforms.lightPositions[i], 1, value_ptr(viewSpaceLightPosition));
				glUniform3fv(ssaoDeferredShadingShaderUniforms.lightColors[i], 1, value_ptr(m_lightColors[i]));
			}
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.position);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.normal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.diffuseColor);
			glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
			if (m_enableBlurringSSAO)
			{
				glBindTexture(GL_TEXTURE_2D, m_ssaoBlurTexID);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_ssaoTexID);
			}
			renderQuad();
		}
		glUseProgram(0);
		/////////////////////////////////////////////////////////////////////////////////
		// write to default frame buffer		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBufferFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, m_screenSize.x, m_screenSize.y, 0, 0, m_screenSize.x, m_screenSize.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/////////////////////////////////////////////////////////////////////////////////
		if (m_showLights)
		{
			glPushMatrix();
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf(value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(value_ptr(m_camera.GetProjectionMatrix(aspect)));
				glPointSize(20.0f);
				glBegin(GL_POINTS);
				{
					for (int i = 0; i < m_lightPositions.size(); ++i)
					{
						glColor4f(m_lightColors[i].r, m_lightColors[i].g, m_lightColors[i].b, 1.0);
						glVertex3d(m_lightPositions[i].x, m_lightPositions[i].y, m_lightPositions[i].z);
					}
				}
				glEnd();
			}
			glPopMatrix();
		}
	}
	else
	{
		glUseProgram(gBufferVisualizationShaderProgram);
		{
			glUniform1f(gBufferVisualizationShaderUniforms.near_plane, CAMERA_NEAR_PLANE);
			glUniform1f(gBufferVisualizationShaderUniforms.far_plane, CAMERA_FAR_PLANE);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.position);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.normal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.diffuseColor);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_gBuffer.depth);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, m_ssaoTexID);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, m_ssaoBlurTexID);
			renderQuad();
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
	printf("resize %d %d\n", width, height);

	m_screenSize = vec2(width, height);
	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);

	resizeBuffers(width, height);

	TwWindowSize(width, height);
}
void My_Timer(int val)
{
	//GLfloat move = glutGet(GLUT_ELAPSED_TIME) / 50.0;

	if (m_autoRotate)
		m_lightPos = glm::rotateY(m_lightPos, m_lightRotateSpeed * 0.01f);

	checkGbufferTypeChanged();

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
	}
}
void My_SpecialKeys(int key, int x, int y)
{
	if (!TwEventSpecialGLUT(key, x, y))
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
	glutCreateWindow("13-5. Ambient Occlusion"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
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
