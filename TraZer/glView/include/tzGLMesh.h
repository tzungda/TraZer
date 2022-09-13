#ifndef TZ_GL_MESH_H
#define TZ_GL_MESH_H

/*
 Draw a line in the GL View
*/

#include "interfaces/tzIGLDraw.h"
#include "Interfaces/tzIGLObject.h"
#include "../include/tzCoreMaterial.h"


class tzIGLMaterial;

//
class tzGLMesh: public tzIGLDraw,
				public tzIGLObject
{
public:
	tzGLMesh();
	virtual ~tzGLMesh( );

/*
my members
*/
private:
	//tzCoreMesh	*mCoreMeshPtr;
	std::vector<float> mPositions;
	std::vector<float> mTexcoords;
	std::vector<float> mNormals;
	std::vector<unsigned int> mIndices;
	unsigned int mIndexCount;
	GLuint			mVBObject[3];
	int				mMaterialId;

	// 
	std::shared_ptr<tzCoreMaterial> mPtrCoreMaterial;

public:
	
	unsigned int vao;
	unsigned int vbo;
	unsigned int vboTex;
	unsigned int ebo;
	unsigned int p_normal;

	unsigned int mTexture;


private:
	void			buildRenderData( );

/*
my interfaces
*/
public:
	void							setMaterialId( int matId );
	void							setVAO( GLint vao );
	void							setVBO( const GLuint vbo[] );
	const std::vector<float>&		positions() const;
	const std::vector<float>&		texcoords() const;
	const std::vector<float>&		normals() const;
	unsigned int					indexCount() const;
	//
	void							setCoreMaterial(std::shared_ptr<tzCoreMaterial> mat );
/*
derived interfaces
*/
public:
	// tzIGLDraw interface
	virtual	void			init(GLuint shaderProgram = 0);
	virtual	void			draw( );

	// tzIObject interface
	virtual	void			setCoreObject( std::shared_ptr<tzCoreObject> coreObjectPtr);
};

#endif
