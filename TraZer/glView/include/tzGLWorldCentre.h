#ifndef TZ_GL_WORLDCENTRE
#define TZ_GL_WORLDCENTRE

#include "../include/tzGLLine.h"

class tzGLWorldCentre
{
public:
	tzGLWorldCentre();
	virtual ~tzGLWorldCentre();

private:
	tzGLLine	mAxisX;
	tzGLLine	mAxisY;
	tzGLLine	mAxisZ;

public: 
	// 
	virtual		void init(GLuint shaderProgram);
	virtual		void draw(GLuint colorId);
};

#endif