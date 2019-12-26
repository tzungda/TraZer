#include "../include/tzGLWorldCentre.h"

#include "../Utilities/tzVector3D.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLWorldCentre::tzGLWorldCentre()
{
	mAxisX.setLineData(tzVector3D(0.0f, 0.0f, 0.0f), tzVector3D(10.0f, 0.0f, 0.0f));
	mAxisY.setLineData(tzVector3D(0.0f, 0.0f, 0.0f), tzVector3D(0.0f, 10.0f, 0.0f));
	mAxisZ.setLineData(tzVector3D(0.0f, 0.0f, 0.0f), tzVector3D(0.0f, 0.0f, 10.0f));
}

//================================================================================
tzGLWorldCentre::~tzGLWorldCentre()
{
}


/*
tzIGLDraw interfaces
*/

//================================================================================
void tzGLWorldCentre::init(GLuint shaderProgram)
{
	// initialize line buffers
	mAxisX.init(shaderProgram);
	mAxisY.init(shaderProgram);
	mAxisZ.init(shaderProgram);
}

//================================================================================
void tzGLWorldCentre::draw( GLuint colorId )
{
	tzVector3D xAxis( 1.0f, 0.0f, 0.0f );
	tzVector3D yAxis( 0.0f, 1.0f, 0.0f );
	tzVector3D zAxis( 0.0f, 0.0f, 1.0f );

	glUniform3fv(colorId, 1, (GLfloat*)(&xAxis.x));
	mAxisX.draw();
	glUniform3fv(colorId, 1, (GLfloat*)(&yAxis.x));
	mAxisY.draw();
	glUniform3fv(colorId, 1, (GLfloat*)(&zAxis.x));
	mAxisZ.draw();
}
