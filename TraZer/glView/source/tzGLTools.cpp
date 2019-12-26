#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>

#include "../include/tzGLTools.h"
#include "interfaces/tzIGLDraw.h"

//===================================================================================
char** tzGLTools::loadShaderSource(const char* file)
{
	FILE* fp = NULL;
	fopen_s(&fp, file, "rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *src = new char[sz + 1];
	fread(src, sizeof(char), sz, fp);
	src[sz] = '\0';
	char **srcp = new char*[1];
	srcp[0] = src;
	return srcp;
}

//===================================================================================
void tzGLTools::freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

//===================================================================================
void tzGLTools::shaderLog(unsigned int shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar* errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		printf("%s\n", errorLog);
		delete[] errorLog;
	}
}

//===================================================================================
unsigned int tzGLTools::loadShader(std::string vsPath, std::string fsPath, std::string gsPath)
{
	bool useGeometricShader = gsPath != "";
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint gs = (useGeometricShader) ? gs = glCreateShader(GL_GEOMETRY_SHADER) : -1;
	char** vsSource = loadShaderSource(vsPath.c_str());
	char** fsSource = loadShaderSource(fsPath.c_str());
	char** gsSource = (useGeometricShader) ? loadShaderSource(gsPath.c_str()) : NULL;
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	if (useGeometricShader) glShaderSource(gs, 1, gsSource, NULL);
	freeShaderSource(vsSource);
	freeShaderSource(fsSource);
	if (useGeometricShader) freeShaderSource(gsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	if (useGeometricShader) glCompileShader(gs);
	shaderLog(vs);
	shaderLog(fs);
	if (useGeometricShader) shaderLog(gs);
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	if (useGeometricShader) glAttachShader(shaderProgram, gs);
	glLinkProgram(shaderProgram);
	return shaderProgram;
	/*
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = loadShaderSource(vsPath.c_str());
	char** fsSource = loadShaderSource(fsPath.c_str());
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	freeShaderSource(vsSource);
	freeShaderSource(fsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	shaderLog(vs);
	shaderLog(fs);
	//Attach Shader to program
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	return shaderProgram;
	*/
}