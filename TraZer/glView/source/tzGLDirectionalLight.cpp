
#include "../include/tzGLDirectionalLight.h"
#include "../include/tzGLTools.h"
#include <string>

#include "../include/tzTool.h"

#include "../include/tzCoreMesh.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLDirectionalLight::tzGLDirectionalLight(): mGLLightMatrix( 0 )
{
	mShadowmapResolution = 512;
	mNearPlane = 0.01f;
	mFarPlane = 50.0f;
	mLightType = directional;
}

/*
interface
*/

//================================================================================
void tzGLDirectionalLight::setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath, std::string geoShaderPath)
{
	// configure depth map FBO for directional light
	// -----------------------
	glGenFramebuffers(1, &mGLLightDepthMapFBO);
	// create depth texture
	glGenTextures(1, &mGLLightShadowDepthMap);
	glBindTexture(GL_TEXTURE_2D, mGLLightShadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowmapResolution, mShadowmapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mGLLightDepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mGLLightShadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//------------------------------------------------------------------------------------------------------------------------------------------------------
	mGLShaderProgram = tzGLTools::loadShader(vertShaderPath, fragShaderPath, geoShaderPath);
	{
		glUseProgram(mGLShaderProgram);

		mGLModelMatrix = glGetUniformLocation(mGLShaderProgram, "model");
		mGLLightMatrix = glGetUniformLocation(mGLShaderProgram, "lightSpaceMatrix");

		glUniform1i(glGetUniformLocation(mGLShaderProgram, "diffuseTexture"), 0);
	}
}

//================================================================================
void tzGLDirectionalLight::updateAttributes()
{
	const float shadowOtho = 15.0f;
	tzMatrix lightProjection, lightView, lightSpaceMatrix;
	lightProjection = tzTool::ortho(-shadowOtho, shadowOtho, -shadowOtho, shadowOtho, mNearPlane, mFarPlane );
	lightView = tzTool::lookAt(mLightPos, tzVector3D(0.0f), tzVector3D(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightView* lightProjection;

	glUseProgram(mGLShaderProgram);
	{
		glUniformMatrix4fv( mGLLightMatrix, 1, GL_FALSE, (GLfloat*)(lightSpaceMatrix.m));
		glViewport(0, 0, mShadowmapResolution, mShadowmapResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, mGLLightDepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < mMeshList.size(); ++i)
		{
			if ( mLightType == 0 && i >= 4 && i <= 7) continue;

			std::shared_ptr<tzGLMesh> mesh = mMeshList[i];
			std::shared_ptr<tzCoreMesh> coreMesh = std::dynamic_pointer_cast<tzCoreMesh>(mesh->coreObject());

			glBindVertexArray(mesh->vao);
			glUniformMatrix4fv( mGLModelMatrix, 1, GL_FALSE, (GLfloat*)(coreMesh->transformMatrix().m));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->mTexture);
			glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glUseProgram(0);
}

//================================================================================
void tzGLDirectionalLight::setCoreObject( std::shared_ptr<tzCoreObject> coreObjectPtr)
{
	this->mPtrCoreObject = coreObjectPtr;

	std::shared_ptr<tzCoreLight> coreLight = std::dynamic_pointer_cast<tzCoreLight>(coreObjectPtr);

	mShadowmapResolution = coreLight->shadowmapResolution();
	mNearPlane = coreLight->nearPlane();
	mFarPlane = coreLight->farPlane();
	mLightPos = coreLight->position();
}

