
#include "../include/tzGLBlinnPhongMaterial_Shadow.h"
#include "../include/tzGLTools.h"
#include "../include/tzTool.h"
#include "../include/tzCoreMaterial.h"
#include "../include/tzMesh.h"
#include "../include/tzCoreMesh.h"

#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLBlinnPhongMaterial_Shadow::tzGLBlinnPhongMaterial_Shadow()
{
	mGLModelMatrix = 0;
	mGLViewMatrix = 0;
	mGLProjectionMatrix = 0;
	mGLLightMatrix = 0;
	mGLFarPlane = 0;
	mGLLightSourceType = 0;
	mGLLightPos = 0;
	mGLViewPos = 0;
	mGLUseShadowmap = 0;
	mGLUseBias = 0;
	mGLPcfKernel = 0;

	//
	mPtrCamera = NULL;
	m_directionalLight_shadowDepthMap = m_pointLight_shadowDepthMap = 0;

	mLightSourceType = 1;
}

//================================================================================
tzGLBlinnPhongMaterial_Shadow::~tzGLBlinnPhongMaterial_Shadow()
{
}

//================================================================================
void tzGLBlinnPhongMaterial_Shadow::updateMaterial()
{
	tzCoreMaterial* coreMat = (tzCoreMaterial*)mPtrCoreObject;
	
}


//================================================================================
void tzGLBlinnPhongMaterial_Shadow::setCoreObject(tzCoreObject *coreObjectPtr)
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
void tzGLBlinnPhongMaterial_Shadow::setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath)
{
	//glUseProgram(0);
	mGLBlinnPhongShadowShaderProgram = tzGLTools::loadShader(vertShaderPath, fragShaderPath);
	{
		glUseProgram(mGLBlinnPhongShadowShaderProgram);

		mGLLightPos = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "lightPos");
		mGLViewPos = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "viewPos");
		mGLModelMatrix = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "model");
		mGLViewMatrix = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "view");
		mGLProjectionMatrix = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "projection");
		mGLLightMatrix = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "lightSpaceMatrix");
		mGLUseShadowmap = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "useShadowMap");
		mGLUseBias = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "useBias");
		mGLPcfKernel = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "pcfKernel");
		mGLLightSourceType = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "lightSourceType");
		mGLFarPlane = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "farPlane");

		glUniform1i(glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "diffuseTexture"), 0);
		glUniform1i(glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "shadowMap_directional"), 1);
		glUniform1i(glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "shadowMap_point"), 2);

		//
		int				m_useShadowmap = 1;
		int				m_useBias = 1;
		int				m_pcfKernel = 1;
		glUniform1i( mGLUseShadowmap, (m_useShadowmap > 0) ? 1 : 0);
		glUniform1i( mGLUseBias, (m_useBias > 0) ? 1 : 0);
		glUniform1i( mGLPcfKernel, m_pcfKernel);
	}
}

//================================================================================
void tzGLBlinnPhongMaterial_Shadow::updateAttributes(const tzMatrix &modelMatrix)
{
	//********************************************
	//int m_lightSource_type = 1;
	tzVector3D mLightPos( 5, 4, 0 );
	float SHADOW_OTHO = 15.0f;
	//float POINT_LIGHT_NEARPLANE = 0.1f;
	float POINT_LIGHT_FARPLANE = 50.0f;
	float DIRECTIONAL_LIGHT_NEARPLANE = 0.01f;
	float DIRECTIONAL_LIGHT_FARPLANE = 50.0f;

	tzMatrix lightProjection, lightView, lightSpaceMatrix;
	lightProjection = tzTool::ortho(-SHADOW_OTHO, SHADOW_OTHO, -SHADOW_OTHO, SHADOW_OTHO, DIRECTIONAL_LIGHT_NEARPLANE, DIRECTIONAL_LIGHT_FARPLANE);
	lightView = tzTool::lookAt(mLightPos, tzVector3D(0.0f), tzVector3D(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightView* lightProjection;
	//********************************************

	tzVector3D eyePos( 0.0f, 0.0f, 5.0f );

	tzVector3D camPos = mPtrCamera->position();
	tzMatrix projMat = mPtrCamera->projectionMatrix();
	tzMatrix view = mPtrCamera->viewMatrix();

	glUseProgram(mGLBlinnPhongShadowShaderProgram);
	{
		glUniform1i( mGLLightSourceType, mLightSourceType);
		glUniform1f( mGLFarPlane, POINT_LIGHT_FARPLANE);
		glUniform3fv( mGLLightPos, 1, (GLfloat*)(&mLightPos.x));
		glUniform3fv( mGLViewPos, 1, (GLfloat*)(&eyePos.x));
		glUniformMatrix4fv( mGLLightMatrix, 1, GL_FALSE, (GLfloat*)(lightSpaceMatrix.m));
		glUniformMatrix4fv( mGLViewMatrix, 1, GL_FALSE, (GLfloat*)(view.m));
		glUniformMatrix4fv( mGLProjectionMatrix, 1, GL_FALSE, (GLfloat*)(projMat.m));

		for (int i = 0; i < mMeshList.size(); ++i)
		{
			if (mLightSourceType == 0 && i >= 4 && i <= 7) continue;

			tzGLMesh *mesh = mMeshList[i];
			tzCoreMesh *coreMesh = (tzCoreMesh*)mesh->coreObject();

			glBindVertexArray(mesh->vao);
			glUniformMatrix4fv( mGLModelMatrix, 1, GL_FALSE, (GLfloat*)(coreMesh->transformMatrix().m));//value_ptr(m_shape.getTransformationMatrix()));
			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, mesh->mTexture);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, m_directionalLight_shadowDepthMap);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, m_pointLight_shadowDepthMap);
			glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	glUseProgram(0);
}

//================================================================================
const std::map<std::string, tzCoreTexture*>& tzGLBlinnPhongMaterial_Shadow::textureList() const
{
	return ((tzCoreMaterial*)mPtrCoreObject)->textureList();
}
