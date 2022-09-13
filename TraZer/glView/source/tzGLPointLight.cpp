
#include "../include/tzGLPointLight.h"
#include "../include/tzGLTools.h"
#include <string>

#include "../include/tzTool.h"

#include "../include/tzCoreMesh.h"


/*
Constructor/Destructor
*/

//================================================================================
tzGLPointLight::tzGLPointLight(): mGLFarPlane( 0 ), mGLLightPos( 0 )
{
	for ( int l = 0; l < 6; l++ )
	{
		mGLShadowMatrices[l] = 0;
	}

	mShadowmapResolution = 512;
	mNearPlane = 0.1f;
	mFarPlane = 50.0f;
	mLightType = point;
}

/*
interface
*/

//================================================================================
void tzGLPointLight::setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath, std::string geoShaderPath)
{
	// configure depth map FBO for point light
	// -----------------------
	glGenFramebuffers(1, &mGLLightDepthMapFBO);
	// create depth texture
	glGenTextures(1, &mGLLightShadowDepthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mGLLightShadowDepthMap);
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, mShadowmapResolution, mShadowmapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mGLLightDepthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mGLLightShadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//------------------------------------------------------------------------------------------------------------------------------------------------------
	mGLShaderProgram = tzGLTools::loadShader(vertShaderPath, fragShaderPath, geoShaderPath);
	{
		glUseProgram(mGLShaderProgram);

		mGLModelMatrix = glGetUniformLocation(mGLShaderProgram, "model");
		mGLLightPos = glGetUniformLocation(mGLShaderProgram, "lightPos");
		mGLFarPlane = glGetUniformLocation(mGLShaderProgram, "farPlane");
		for (int i = 0; i < 6; ++i)
		{
			mGLShadowMatrices[i] = glGetUniformLocation(mGLShaderProgram, std::string("shadowMatrices[" + std::to_string(i) + "]").c_str());
		}

		glUniform1i(glGetUniformLocation(mGLShaderProgram, "diffuseTexture"), 0);
	}
}

//================================================================================
void tzGLPointLight::updateAttributes()
{
	// create depth cubemap transformation matrices (shadow matrices)
	tzMatrix shadowProj = tzTool::perspective( 90.0f, (float)mShadowmapResolution / (float)mShadowmapResolution, mNearPlane, mFarPlane );
	std::vector<tzMatrix> shadowTransforms(6);
	shadowTransforms[0] = tzTool::lookAt(mLightPos, mLightPos + tzVector3D(1.0f, 0.0f, 0.0f), tzVector3D(0.0f, -1.0f, 0.0f))*shadowProj;
	shadowTransforms[1] = tzTool::lookAt(mLightPos, mLightPos + tzVector3D(-1.0f, 0.0f, 0.0f), tzVector3D(0.0f, -1.0f, 0.0f))*shadowProj;
	shadowTransforms[2] = tzTool::lookAt(mLightPos, mLightPos + tzVector3D(0.0f, 1.0f, 0.0f), tzVector3D(0.0f, 0.0f, 1.0f))*shadowProj;
	shadowTransforms[3] = tzTool::lookAt(mLightPos, mLightPos + tzVector3D(0.0f, -1.0f, 0.0f), tzVector3D(0.0f, 0.0f, -1.0f))*shadowProj;
	shadowTransforms[4] = tzTool::lookAt(mLightPos, mLightPos + tzVector3D(0.0f, 0.0f, 1.0f), tzVector3D(0.0f, -1.0f, 0.0f))*shadowProj;
	shadowTransforms[5] = tzTool::lookAt(mLightPos, mLightPos + tzVector3D(0.0f, 0.0f, -1.0f), tzVector3D(0.0f, -1.0f, 0.0f))*shadowProj;

	glUseProgram(mGLShaderProgram);
	{
		for (int i = 0; i < 6; ++i)
		{
			glUniformMatrix4fv(mGLShadowMatrices[i], 1, GL_FALSE, (GLfloat*)(shadowTransforms[i].m));
		}
		glUniform1f( mGLFarPlane, mFarPlane);
		glUniform3fv( mGLLightPos, 1, (GLfloat*)(&mLightPos.x));

		glViewport(0, 0, mShadowmapResolution, mShadowmapResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, mGLLightDepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < mMeshList.size(); ++i)
		{
			std::shared_ptr<tzGLMesh> mesh = mMeshList[i];
			std::shared_ptr<tzCoreMesh> coreMesh = std::dynamic_pointer_cast<tzCoreMesh>(mesh->coreObject());

			glBindVertexArray(mesh->vao);
			glUniformMatrix4fv( mGLModelMatrix, 1, GL_FALSE, (GLfloat*)(coreMesh->transformMatrix().m) );//value_ptr(m_shape.getTransformationMatrix()));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->mTexture );
			glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glUseProgram(0);
}

//================================================================================
void tzGLPointLight::setCoreObject(std::shared_ptr<tzCoreObject> coreObjectPtr)
{
	this->mPtrCoreObject = coreObjectPtr;

	std::shared_ptr<tzCoreLight> coreLight = std::dynamic_pointer_cast<tzCoreLight>(coreObjectPtr);

	mShadowmapResolution = coreLight->shadowmapResolution();
	mNearPlane = coreLight->nearPlane();
	mFarPlane = coreLight->farPlane();
	mLightPos = coreLight->position();
}

