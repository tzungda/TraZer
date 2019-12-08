#ifndef TZ_GL_TOOLS_H
#define TZ_GL_TOOLS_H

/*
 Draw a line in the GL View
*/



//
class tzGLTools
{
public:
	static char**			loadShaderSource(const char* file);
	static void				freeShaderSource(char** srcp);
	static void				shaderLog(unsigned int shader);
	static unsigned int		loadShader(std::string vsPath, std::string fsPath);
};

#endif
