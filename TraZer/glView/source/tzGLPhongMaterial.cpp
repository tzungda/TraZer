#include "../include/tzGLPhongMaterial.h"
#include "../include/tzGLTools.h"
#include "../include/tzTool.h"
#include "../include/tzCoreMaterial.h"
#include "../include/tzGLMesh.h"

#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLPhongMaterial::tzGLPhongMaterial()
{
	mIllum = 0;
	mDissolve = 1.0f;
	mShininess = 10.0f;
	mIor = 2.0f;

	mGLTexAmbient = 0;
	mGLTexDiffuse = 0;
	mGLTexSpecular = 0;
	mGLTexSpecularHighlight = 0;
	mGLTexBump = 0;
	mGLTexAlpha = 0;

	mHasAmbientTex = 0;
	mHasDiffuseTex = 0;
	mHasSpecularTex = 0;

	mGLPhongShaderProgram = 0;
}

//================================================================================
tzGLPhongMaterial::~tzGLPhongMaterial()
{
}

//================================================================================
void tzGLPhongMaterial::updateMaterial()
{
	tzCoreMaterial* coreMat = (tzCoreMaterial*)mPtrCoreObject;
	this->mName = coreMat->name();
	// colors
	this->mAmbient = coreMat->ambientColor();
	this->mDiffuse = coreMat->diffuseColor();
	this->mSpecular = coreMat->specularColor();
	// textures
	const std::map<std::string, tzCoreTexture*> textureList = coreMat->textureList();
	if (textureList.find("ambient") != textureList.end())
	{
		mHasAmbientTex = 1;
		mAmbientTexPath = textureList.find("ambient")->second->path();
	}
	if (textureList.find("diffuse") != textureList.end())
	{
		mHasDiffuseTex = 1;
		mDiffuseTexPath = textureList.find("diffuse")->second->path();
	}
	if (textureList.find("specular") != textureList.end())
	{
		mHasSpecularTex = 1;
		mSpecularTexPath = textureList.find("specular")->second->path();
	}
}


//================================================================================
void tzGLPhongMaterial::setCoreObject(tzCoreObject *coreObjectPtr)
{
	mPtrCoreObject = coreObjectPtr;
	if (!mPtrCoreObject)
	{
		printf(" The given material is null \n");
		return;
	}

	updateMaterial();
}

//================================================================================
void tzGLPhongMaterial::setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath)
{
	glUseProgram(0);
	mGLPhongShaderProgram = tzGLTools::loadShader(vertShaderPath, fragShaderPath);
	{
		glUseProgram(mGLPhongShaderProgram);

		//Cache uniform variable id
		mGLModelMatrix = glGetUniformLocation(mGLPhongShaderProgram, "model");
		mGLViewMatrix = glGetUniformLocation(mGLPhongShaderProgram, "view");
		mGLProjectionMatrix = glGetUniformLocation(mGLPhongShaderProgram, "projection");
		mGLLightMatrix = glGetUniformLocation(mGLPhongShaderProgram, "lightSpaceMatrix");
		mGLLightPos = glGetUniformLocation(mGLPhongShaderProgram, "lightPos");
		mGLViewPos = glGetUniformLocation(mGLPhongShaderProgram, "viewPos");
		mGLAmbient = glGetUniformLocation(mGLPhongShaderProgram, "ambient");
		mGLDiffuse = glGetUniformLocation(mGLPhongShaderProgram, "diffuse");
		mGLSpecular = glGetUniformLocation(mGLPhongShaderProgram, "specular");
		mGLShininess = glGetUniformLocation(mGLPhongShaderProgram, "shininess");

		mGLHasAmbientTex = glGetUniformLocation(mGLPhongShaderProgram, "hasAmbientTexture");
		mGLHasDiffuseTex = glGetUniformLocation(mGLPhongShaderProgram, "hasDiffuseTexture");
		mGLHasSpecularTex = glGetUniformLocation(mGLPhongShaderProgram, "hasSpecularTexture");

		glUniform1i(glGetUniformLocation(mGLPhongShaderProgram, "ambientTexture"), 0);
		glUniform1i(glGetUniformLocation(mGLPhongShaderProgram, "diffuseTexture"), 1);
		glUniform1i(glGetUniformLocation(mGLPhongShaderProgram, "specularTexture"), 2);
	}


	// generate textures
	tzTextureData textureData;
	textureData = tzTool::loadPngTexture(mAmbientTexPath.c_str());
	if (mHasAmbientTex && textureData.data)
	{
		glGenTextures(1, &mGLTexAmbient);
		glBindTexture(GL_TEXTURE_2D, mGLTexAmbient);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	textureData = tzTool::loadPngTexture(mDiffuseTexPath.c_str());
	if (mHasDiffuseTex && textureData.data != NULL)
	{
		glGenTextures(1, &mGLTexDiffuse);
		glBindTexture(GL_TEXTURE_2D, mGLTexDiffuse);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	textureData = tzTool::loadPngTexture(mSpecularTexPath.c_str());
	if (mHasSpecularTex && textureData.data)
	{
		glGenTextures(1, &mGLTexSpecular);
		glBindTexture(GL_TEXTURE_2D, mGLTexSpecular);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

//================================================================================
void tzGLPhongMaterial::updateAttributes(const tzMatrix &modelMatrix)
{
	glUniformMatrix4fv( mGLModelMatrix, 1, GL_FALSE, (GLfloat*)modelMatrix.m);
	
	glUniform3fv( mGLAmbient, 1, &(mAmbient.x) );
	glUniform3fv( mGLDiffuse, 1, &(mDiffuse.x));
	glUniform3fv( mGLSpecular, 1, &(mSpecular.x));
	glUniform1f( mGLShininess, mShininess);

	glUniform1i(mGLHasAmbientTex, mHasAmbientTex);
	if (mHasAmbientTex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mGLTexAmbient);
	}
	glUniform1i(mGLHasDiffuseTex, mHasDiffuseTex);
	if (mHasDiffuseTex)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mGLTexDiffuse);
	}
	glUniform1i(mGLHasSpecularTex, mHasSpecularTex);
	if (mHasSpecularTex)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mGLTexSpecular);
	}
}

//================================================================================
const std::map<std::string, tzCoreTexture*>& tzGLPhongMaterial::textureList() const
{
	return ((tzCoreMaterial*)mPtrCoreObject)->textureList();
}
