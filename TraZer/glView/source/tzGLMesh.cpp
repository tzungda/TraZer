#include "../include/tzGLMesh.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLMesh::tzGLMesh()
{
	this->mInitialized = false;
	mCoreMeshPtr = NULL;
	mIndexCount = 0;
}

//================================================================================
tzGLMesh::~tzGLMesh()
{
	if ( mInitialized )
	{
		glDeleteBuffers( 1, &mVAObject );
		glDeleteBuffers( 3, mVBObject );
		glDeleteShader( mShaderProgram );
	}
}


/*
my interfaces
*/

//================================================================================
void tzGLMesh::buildRenderData()
{
	if ( !mCoreMeshPtr )
	{
		printf( "Can't build render data because the mesh is NULL \n" );
		return;
	}

	mName = mCoreMeshPtr->name();

	//
	int indexLen = (int)mCoreMeshPtr->indices().size();
	mPositions.resize(indexLen * 3);
	mNormals.resize(indexLen * 3);
	mTexcoords.resize(indexLen * 2);
	//
	mIndexCount = indexLen;//->numVertices();
								  //
								  //int index = 0;
	for (int ind = 0; ind < indexLen; ind++)
	{
		int index = ind * 3;
		int index2 = ind * 2;
		int vid = mCoreMeshPtr->indices()[ind].vertex_index;
		int nid = mCoreMeshPtr->indices()[ind].normal_index;
		int uvid = mCoreMeshPtr->indices()[ind].texcoord_index;
		//
		tzPoint3D p = mCoreMeshPtr->vertices()[vid];
		mPositions[index] = p.x;
		mPositions[index + 1] = p.y;
		mPositions[index + 2] = p.z;
		//
		tzNormal n = mCoreMeshPtr->vertexNormals()[nid];
		mNormals[index] = n.x;
		mNormals[index + 1] = n.y;
		mNormals[index + 2] = n.z;
		//
		mTexcoords[index2] = mCoreMeshPtr->us()[uvid];
		mTexcoords[index2 + 1] = mCoreMeshPtr->vs()[uvid];
	}
}

//================================================================================
void tzGLMesh::setMesh(const tzCoreMesh *coreMeshPtr)
{
	mCoreMeshPtr = (tzCoreMesh*)coreMeshPtr;

	// build mesh data
	buildRenderData();
}


/*
tzIGLDraw interfaces
*/

//================================================================================
void tzGLMesh::init(GLuint shaderProgram)
{
	if (mInitialized || !mCoreMeshPtr )
		return;

	glGenVertexArrays(1, &mVAObject);
	glBindVertexArray(mVAObject);

	glGenBuffers(3, mVBObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVBObject[0]);
	glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(float), mPositions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBObject[1]);
	glBufferData(GL_ARRAY_BUFFER, mTexcoords.size() * sizeof(float), mTexcoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBObject[2]);
	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), mNormals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//
	this->mInitialized = true;
}

//================================================================================
void tzGLMesh::draw()
{
	if (!mInitialized)
		return;
	//glm::mat4 mtx;

	//glUseProgram(lineShaderProgram);

	glBindVertexArray(mVAObject);

	//glUniformMatrix4fv(lineShaderPrograms.model_matrix, 1, GL_FALSE, value_ptr(mtx));

	glDrawArrays(GL_TRIANGLES, 0, (int)mCoreMeshPtr->indices().size());
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}