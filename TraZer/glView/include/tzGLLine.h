#ifndef TZ_GL_LINE_H
#define TZ_GL_LINE_H

/*
 Draw a line in the GL View
*/

#include "interfaces/tzIGLDraw.h"

class tzVector3D;

struct lineShaderParams
{
	GLint model_matrix;
	GLint view_matrix;
	GLint projection_matrix;
	GLint line_color;
} ;

//
class tzGLLine: public tzIGLDraw
{
public:
	tzGLLine();
	virtual ~tzGLLine( );

/*
my members
*/
private:
	std::vector<tzVector3D>	mLintPoints;
	lineShaderParams		mLineShaderParams;
	GLuint			mVBObject;

/*
my interfaces
*/
public:
	void			setLineData(const tzVector3D& startPoint, const tzVector3D& endPoint );

/*
tzIGLDraw interfaces
*/
public:
	virtual			void init(GLuint shaderProgram = 0);
	virtual			void draw( );
};

#endif
