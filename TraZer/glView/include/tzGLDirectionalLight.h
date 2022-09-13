#ifndef TZ_GL_DIRECTIONALLIGHT_H
#define TZ_GL_DIRECTIONALLIGHT_H

/*
 Draw a line in the GL View
*/

#include "Interfaces/tzIGLLight.h"
#include "../include/tzCoreCamera.h"

//
class tzGLDirectionalLight : public tzIGLLight,
							 public tzIGLObject
{
public:
	tzGLDirectionalLight();
	virtual ~tzGLDirectionalLight( )
	{
	}

/*
member data
*/
public:
	
	int					mGLLightMatrix;
	//
	//unsigned int		mGLDirectionalLightDepthShaderProgram;
	//
	//unsigned int		mGLDirectionalLightDepthMapFBO;
	//
	//unsigned int		mDirectionalLightShadowDepthMap;
	//
	


	//tzCoreCamera mCamera;
/*
tzIGLLight interfaces
*/
public:
	virtual void		setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath, std::string geoShaderPath = "");
	virtual void		updateAttributes();
public:
	// tzIGLObject interface
	virtual	void		setCoreObject( std::shared_ptr<tzCoreObject> coreObjectPtr);
};

#endif
