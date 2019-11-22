#ifndef TZ_GL_MESH_H
#define TZ_GL_MESH_H

/*
 Draw a line in the GL View
*/

#include "interfaces/tzIGLDraw.h"
#include "../include/tzCoreMesh.h"
#include "GLM/glm/glm.hpp"


//
class tzGLMesh: public tzIGLDraw
{
public:
	tzGLMesh();
	virtual ~tzGLMesh( );

/*
my members
*/
private:
	tzCoreMesh	*mCoreMeshPtr;
	std::vector<float> mPositions;
	std::vector<float> mTexcoords;
	std::vector<float> mNormals;
	unsigned int mIndexCount;
	GLuint			mVBObject[3];

	/*
	my members
	*/
private:
	void			buildRenderData( );

/*
my interfaces
*/
public:
	void			setMesh( const tzCoreMesh *coreMeshPtr );

/*
tzIGLDraw interfaces
*/
public:
	virtual			void init( GLuint shaderProgram );
	virtual			void draw( );
};

#endif
