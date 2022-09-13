
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
	mShadowOtho = 15.0f;
	mPointLightFarPlane = 50.0f;
	mDirectionalLightNearPlane = 0.01f;
	mDirectionalLightFarPlane = 50.0f;

	//
	mPointLightPos = mDirectionalLightPos = tzVector3D( 5.0f, 4.0f, 0.0f );
	mLightProjection = tzTool::ortho(-mShadowOtho, mShadowOtho, -mShadowOtho, mShadowOtho, mDirectionalLightNearPlane, mDirectionalLightFarPlane);
	mLightView = tzTool::lookAt(mDirectionalLightPos, tzVector3D(0.0f), tzVector3D(0.0, 1.0, 0.0));
	mLightSpaceMatrix = mLightView * mLightProjection;

	//
	mGLModelMatrix = 0;
	mGLViewMatrix = 0;
	mGLProjectionMatrix = 0;
	mGLLightMatrix = 0;
	mGLFarPlane = 0;
	mGLLightSourceType = 0;
	mGLPointLightPos = 0;
	mGLDirectionalLightPos = 0;
	mGLViewPos = 0;
	mGLUseShadowmap = 0;
	mGLUseBias = 0;
	mGLPcfKernel = 0;

	//
	mPtrCamera = nullptr;
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
}


//================================================================================
void tzGLBlinnPhongMaterial_Shadow::setCoreObject( std::shared_ptr< tzCoreObject > coreObjectPtr)
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

		mGLPointLightPos = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "pointLightPos");
		mGLDirectionalLightPos = glGetUniformLocation(mGLBlinnPhongShadowShaderProgram, "directionalLightPos");
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
	tzVector3D camPos = mPtrCamera->position();

	tzMatrix projMat = mPtrCamera->projectionMatrix();
	tzMatrix view = mPtrCamera->viewMatrix();

	// test
	//tzVector3D directionalLightPos( 4.0f, 50.0f, 0.0f );
	// test

	glUseProgram(mGLBlinnPhongShadowShaderProgram);
	{
		glUniform1i( mGLLightSourceType, mLightSourceType);
		glUniform1f( mGLFarPlane, mPointLightFarPlane);
		glUniform3fv(mGLPointLightPos, 1, (GLfloat*)(&mPointLightPos.x));
		glUniform3fv(mGLDirectionalLightPos, 1, (GLfloat*)(&mDirectionalLightPos.x));
		glUniform3fv( mGLViewPos, 1, (GLfloat*)(&camPos.x));
		glUniformMatrix4fv( mGLLightMatrix, 1, GL_FALSE, (GLfloat*)(mLightSpaceMatrix.m));
		glUniformMatrix4fv( mGLViewMatrix, 1, GL_FALSE, (GLfloat*)(view.m));
		glUniformMatrix4fv( mGLProjectionMatrix, 1, GL_FALSE, (GLfloat*)(projMat.m));

		for (int i = 0; i < mMeshList.size(); ++i)
		{
			std::shared_ptr<tzGLMesh> mesh = mMeshList[i];
			std::shared_ptr<tzCoreMesh> coreMesh = std::dynamic_pointer_cast<tzCoreMesh>(mesh->coreObject());

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
const std::map<std::string, std::shared_ptr<tzCoreTexture>>& tzGLBlinnPhongMaterial_Shadow::textureList() const
{
	return (std::dynamic_pointer_cast<tzCoreMaterial>(mPtrCoreObject) )->textureList();
}

//================================================================================
void tzGLBlinnPhongMaterial_Shadow::setDirectionalLightPosition( const tzVector3D &directionalLightPos)
{
	mDirectionalLightPos = directionalLightPos;
	mLightView = tzTool::lookAt(mDirectionalLightPos, tzVector3D(0.0f), tzVector3D(0.0, 1.0, 0.0));
	mLightSpaceMatrix = mLightView * mLightProjection;
}

//================================================================================
void tzGLBlinnPhongMaterial_Shadow::setPointLightPosition(const tzVector3D &pointLightPos)
{
	mPointLightPos = pointLightPos;
}

