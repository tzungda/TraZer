#include "../include/tzGLWorldCentre.h"
#include "GLM/glm/gtc/type_ptr.hpp"

/*
Constructor/Destructor
*/

//================================================================================
tzGLWorldCentre::tzGLWorldCentre()
{
	mAxisX.setLineData(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.0f, 0.0f));
	mAxisY.setLineData(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f));
	mAxisZ.setLineData(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 20.0f));
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
	glUniform3fv(colorId, 1, value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
	mAxisX.draw();
	glUniform3fv(colorId, 1, value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
	mAxisY.draw();
	glUniform3fv(colorId, 1, value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
	mAxisZ.draw();
}
