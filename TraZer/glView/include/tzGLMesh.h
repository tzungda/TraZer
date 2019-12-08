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
	GLuint			mVAObject;
	int				mMaterialId;

	void*			mPtrMaterial;

	/*
	my members
	*/
private:
	void			buildRenderData( );

/*
my interfaces
*/
public:
	void							setMesh( const tzCoreMesh *coreMeshPtr );
	void							setMaterialId( int matId );
	void							setVAO( GLint vao );
	void							setVBO( const GLuint vbo[] );
	const std::vector<float>&		positions() const;
	const std::vector<float>&		texcoords() const;
	const std::vector<float>&		normals() const;
	unsigned int					indexCount() const;
	//
	void							setMaterial( void* mat );
	void*							material( ) const;
/*
tzIGLDraw interfaces
*/
public:
	virtual			void init( GLuint shaderProgram );
	virtual			void draw( );
};

#endif
