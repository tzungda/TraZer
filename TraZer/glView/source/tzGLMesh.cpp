
#include "../include/tzGLMesh.h"
#include "../include/tzCoreMesh.h"
#include "../include/tzCoreMaterial.h"

#include "interfaces/tzIGLMaterial.h"

#include "../../Include/tzTool.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLMesh::tzGLMesh()
{
	this->mInitialized = false;
	mIndexCount = 0;
	//mPtrMaterial = NULL;
	mPtrCoreMaterial = nullptr;
	mTexture = 0;
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
	if ( !mPtrCoreObject )
	{
		printf( "Can't build render data because the mesh is NULL \n" );
		return;
	}

	std::shared_ptr<tzCoreMesh> coreMesh = std::dynamic_pointer_cast<tzCoreMesh>(mPtrCoreObject);
	coreMesh->updateData();
	mPositions = coreMesh->floatVertices();
	mNormals = coreMesh->floatNormals();
	mTexcoords = coreMesh->floatTexcoords();
	coreMesh->indices(mIndices);
	mIndexCount = (unsigned int)mIndices.size();


	/*
	// get indices
	coreMesh->indices( mIndices );

	//
	int indexLen = (int)coreMesh->indices().size();
	mPositions.resize(indexLen * 3);
	mNormals.resize(indexLen * 3);
	mTexcoords.resize(indexLen * 2);
	//
	//mIndexCount = indexLen;//->numVertices();
	mIndexCount = (int)mIndices.size();
								  //
								  //int index = 0;
	for (int ind = 0; ind < indexLen; ind++)
	{
		int index = ind * 3;
		int index2 = ind * 2;
		int vid = coreMesh->indices()[ind].vertex_index;
		int nid = coreMesh->indices()[ind].normal_index;
		int uvid = coreMesh->indices()[ind].texcoord_index;
		//
		tzPoint3D p = coreMesh->vertices()[vid];
		mPositions[index] = p.x;
		mPositions[index + 1] = p.y;
		mPositions[index + 2] = p.z;
		//
		tzNormal n = coreMesh->vertexNormals()[nid];
		mNormals[index] = n.x;
		mNormals[index + 1] = n.y;
		mNormals[index + 2] = n.z;
		//
		mTexcoords[index2] = coreMesh->us()[uvid];
		mTexcoords[index2 + 1] = coreMesh->vs()[uvid];
	}
	*/
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
void tzGLMesh::setCoreMaterial(std::shared_ptr<tzCoreMaterial> mat )
{
	mPtrCoreMaterial = mat;
}


/*
tzIGLDraw interfaces
*/

//================================================================================
void tzGLMesh::init(GLuint shaderProgram)
{
	if (mInitialized || !mPtrCoreObject )
		return;

	if( !mPtrCoreMaterial)
	{
		printf( "\n\n !!!!! the mesh doesn't have material \n\n" );
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, &vbo);
	glGenBuffers(1, &p_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(float) + mNormals.size() * sizeof(float), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, mPositions.size() * sizeof(float), &mPositions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(float), mNormals.size() * sizeof(float), &mNormals[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)(mPositions.size() * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, p_normal);
	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), mNormals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboTex);
	glBufferData(GL_ARRAY_BUFFER, mTexcoords.size() * sizeof(float), mTexcoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);
	//m_shape.materialId = shapes[j].mesh.material_ids[0];
	//m_shape.indexCount = shapes[j].mesh.indices.size();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	/*
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
	*/
	
	const std::map<std::string, std::shared_ptr<tzCoreTexture>>& texList = mPtrCoreMaterial->textureList();
	std::string texPath = texList.find("diffuse")->second->path();
	tzTextureData textureData;
	textureData = tzTool::loadPngTexture(texPath.c_str());

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//
	this->mInitialized = true;
}

//================================================================================
void tzGLMesh::draw()
{
	/*
	if (!mInitialized)
		return;

	tzCoreMesh *coreMesh = (tzCoreMesh*)mPtrCoreObject;
	
	// update material attributes
	tzIGLMaterial *mat = (tzIGLMaterial*)this->mPtrMaterial;
	if ( mat )
	{
		mat->updateAttributes(coreMesh->transformMatrix());
	}
	
	glBindVertexArray(mVAObject);
	glDrawArrays(GL_TRIANGLES, 0, (int)coreMesh->indices().size());

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	*/
}

//================================================================================
void tzGLMesh::setCoreObject(std::shared_ptr<tzCoreObject> coreObjectPtr)
{
	this->mPtrCoreObject = coreObjectPtr;
	buildRenderData();
}

