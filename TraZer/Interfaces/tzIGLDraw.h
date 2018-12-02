#ifndef TZ_GL_DRAW
#define TZ_GL_DRAW

//#include "Common.h"
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include <vector>
using namespace std;

class tzIGLDraw
{
protected:
	GLuint			mVAObject;
	GLuint			mVBObject;
	GLuint			mShaderProgram;
	char			mInitialized;
	string			mShaderName;

public:
	virtual			string shaderName( ) const {return mShaderName;}
	virtual			void setShaderName( string shader ) { mShaderName = shader; }
	virtual			void init( ) = 0;
	virtual			void draw( ) = 0;
};

#endif