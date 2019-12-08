#include "../include/tzGLMesh.h"
#include "../include/tzGLPhongMaterial.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLMesh::tzGLMesh()
{
	this->mInitialized = false;
	mCoreMeshPtr = NULL;
	mIndexCount = 0;
	mPtrMaterial = NULL;
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

//================================================================================
void tzGLMesh::setMaterialId(int matId)
{
	this->mMaterialId = matId;
}

//================================================================================
const std::vector<float>& tzGLMesh::positions() const
{
	return this->mPositions;
}

//================================================================================
void tzGLMesh::setVAO(GLint vao)
{
	this->mVAObject = vao;
}

//================================================================================
void tzGLMesh::setVBO(const GLuint vbo[])
{
	this->mVBObject[0] = vbo[0];
	this->mVBObject[1] = vbo[1];
	this->mVBObject[2] = vbo[2];
}

//================================================================================
const std::vector<float>& tzGLMesh::texcoords() const
{
	return this->mTexcoords;
}

//================================================================================
const std::vector<float>& tzGLMesh::normals() const
{
	return this->mNormals;
}

//================================================================================
unsigned int  tzGLMesh::indexCount() const
{
	return this->mIndexCount;
}

//================================================================================
void tzGLMesh::setMaterial( void* mat )
{
	mPtrMaterial = mat;
}

//================================================================================
void* tzGLMesh::material() const
{
	return mPtrMaterial;
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
	
	// test......
	tzGLPhongMaterial *mat = (tzGLPhongMaterial *)this->mPtrMaterial;

	glUniformMatrix4fv(mat->mGLModelMatrix, 1, GL_FALSE, (GLfloat*)this->mCoreMeshPtr->transformMatrix().m );
	float ccc[3] = {1.0f, 1.0f, 1.0f};
	glUniform3fv( mat->mGLAmbient, 1,  &(mat->mAmbient.x) );
	glUniform3fv( mat->mGLDiffuse, 1, &(mat->mDiffuse.x) );
	glUniform3fv( mat->mGLSpecular, 1, &(mat->mSpecular.x));
	glUniform1f(mat->mGLShininess, (mat->mShininess));

	glUniform1i(mat->mGLHasAmbientTex, mat->mHasAmbientTex);
	if (mat->mHasAmbientTex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mat->mGLTexAmbient);
	}
	glUniform1i(mat->mGLHasDiffuseTex, mat->mHasDiffuseTex);
	if (mat->mHasDiffuseTex)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mat->mGLTexDiffuse);
	}
	glUniform1i(mat->mGLHasSpecularTex, mat->mHasSpecularTex);
	if (mat->mHasSpecularTex)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mat->mGLTexSpecular);
	}

	glBindVertexArray(mVAObject);
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);
	//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, (int)this->mCoreMeshPtr->indices().size());

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}