#ifndef TZ_GL_DRAW_INTERFACE
#define TZ_GL_DRAW_INTERFACE

//#include "Common.h"
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include <vector>

class tzIGLDraw
{
protected:
	GLuint			mVAObject;
	GLuint			mShaderProgram;
	char			mInitialized;
	std::string			mShaderName;
	//std::string			mName;

public:
	virtual			std::string shaderName( ) const {return mShaderName;}
	virtual			void setShaderName( std::string shader ) { mShaderName = shader; }
	virtual			void init(GLuint shaderProgram = 0) = 0;
	virtual			void draw( ) = 0;
};

#endif