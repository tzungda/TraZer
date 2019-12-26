#ifndef TZ_GL_POINTLIGHT_H
#define TZ_GL_POINTLIGHT_H

/*
 Draw a line in the GL View
*/

#include "Interfaces/tzIGLLight.h"
#include "../include/tzCoreCamera.h"

//
class tzGLPointLight : public tzIGLLight,
					   public tzIGLObject
{
public:
	tzGLPointLight();
	virtual ~tzGLPointLight( )
	{
	}

/*
member data
*/
public:
	

	int					mGLFarPlane;
	int					mGLLightPos;
	int					mGLShadowMatrices[6];
	//
	//unsigned int		mGLPointLightDepthShaderProgram;
	//
	//unsigned int		mPointLightDepthMapFBO;
	//
	//unsigned int		mPointLightShadowDepthMap;
	//
	//int					mShadowmapResolution;


	//tzCoreCamera mCamera;
/*
tzIGLLight interfaces
*/
public:
	virtual void		setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath, std::string geoShaderPath = "");
	virtual void		updateAttributes();

public:
	// tzIGLObject interface
	virtual	void		setCoreObject(tzCoreObject *coreObjectPtr);
};

#endif
