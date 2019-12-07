#ifndef COMMON_H
#define COMMON_H

#ifdef _MSC_VER
	#include "GLEW/glew.h"
	#include "FreeGLUT/freeglut.h"
	#include <direct.h>
#else
	#include <OpenGL/gl3.h>
	#include <GLUT/glut.h>
	#include <unistd.h>
#endif

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "../Include/TinyOBJ/tiny_obj_loader.h"
#endif

#define GLM_SWIZZLE
#include "../Include/GLM/glm/glm.hpp"
#include "../Include/GLM/glm/gtc/matrix_transform.hpp"
#include "../Include/GLM/glm/gtc/type_ptr.hpp"
#include "../Include/GLM/glm/gtx/rotate_vector.hpp"

#ifdef _MSC_VER
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	#define __FILEPATH__(x) ((std::string(__FILE__).substr(0, std::string(__FILE__).rfind('\\'))+(x)).c_str())
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
	#define __FILEPATH__(x) ((std::string(__FILE__).substr(0, std::string(__FILE__).rfind('/'))+(x)).c_str())
#endif

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream> 


#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
#define rad2deg(x) ((180.0f) / ((x)*(3.1415926f)))

typedef struct _TextureData
{
	_TextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} TextureData;

typedef struct _Shape
{
	GLuint vao;
	GLuint vbo;
	GLuint vboTex;
	GLuint ebo;

	GLuint p_normal;
	int materialId;
	int indexCount;
	GLuint m_texture;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 getTransformationMatrix();

} Shape;

class Common
{
public:
	static void DumpInfo(void);
	static void ShaderLog(GLuint shader);
	static void PrintGLError();
	static TextureData Load_png(const char* path);
	static char** LoadShaderSource(const char* file);
	static void FreeShaderSource(char** srcp);
};

#endif  // COMMON_H
