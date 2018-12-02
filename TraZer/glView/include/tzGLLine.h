#ifndef TZ_GL_LINE_H
#define TZ_GL_LINE_H

/*
 Draw a line in the GL View
*/

#include "interfaces/tzIGLDraw.h"
#include "GLM/glm/glm.hpp"

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
	std::vector<glm::vec3>	mLintPoints;
	lineShaderParams		mLineShaderParams;

/*
my interfaces
*/
public:
	void			setLineData( glm::vec3 startPoint, glm::vec3 endPoint );

/*
tzIGLDraw interfaces
*/
public:
	virtual			void init( );
	virtual			void draw( );
};

#endif
