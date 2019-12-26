#ifndef TZ_GL_BLINN_PHONG_MATERIAL_SHADOW_H
#define TZ_GL_BLINN_PHONG_MATERIAL_SHADOW_H

/*
 Draw a line in the GL View
*/

#include "../../Include/tzCoreMaterial.h"
#include "tzGLCamera.h"
#include "Interfaces/tzIGLObject.h"
#include "Interfaces/tzIGLMaterial.h"


// (see http://www.fileformat.info/format/material/)
class tzGLBlinnPhongMaterial_Shadow : public tzIGLObject,
						  public tzIGLMaterial
{

public:
	tzGLBlinnPhongMaterial_Shadow();
	virtual ~tzGLBlinnPhongMaterial_Shadow( );

/*
my members
*/

public:
	int  mGLModelMatrix;
	int  mGLViewMatrix;
	int  mGLProjectionMatrix;
	int  mGLLightMatrix;
	int  mGLFarPlane;
	int  mGLLightSourceType;
	int  mGLLightPos;
	int  mGLViewPos;
	int  mGLUseShadowmap;
	int  mGLUseBias;
	int  mGLPcfKernel;

	//
	unsigned int		mGLBlinnPhongShadowShaderProgram;

	//
	void				updateMaterial();

/*
my interfaces
*/
public:
	// test
	tzGLCamera *mPtrCamera;
	unsigned int m_directionalLight_shadowDepthMap;
	unsigned int m_pointLight_shadowDepthMap;
	//void setCoreCamera( tzCoreCamera *ptrCoreCamera );

	unsigned int mLightSourceType;

/*
derived interfaces
*/
public:
	// tzIGLObject interface
	virtual	void			setCoreObject(tzCoreObject *coreObjectPtr);

	// tzIGLMaterial interface
	virtual void									setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath);
	virtual void									updateAttributes(const tzMatrix &modelMatrix);
	const std::map<std::string, tzCoreTexture*>&	textureList() const;
	

};

#endif
