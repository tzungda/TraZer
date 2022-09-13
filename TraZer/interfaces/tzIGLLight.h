#ifndef TZ_GL_LIGHT_INTERFACE
#define TZ_GL_LIGHT_INTERFACE

//#include "Common.h"
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include <vector>

#include "tzMatrix.h"

#include "../glView/include/tzGLMesh.h"
#include "../Include/tzCoreTypes.h"
#include "../Include/tzCoreLight.h"

class tzIGLLight
{
public:

	tzIGLLight( )
	{ 
		mGLModelMatrix = 0;
		mGLShaderProgram = 0;
		mGLLightDepthMapFBO = 0;
		mGLLightShadowDepthMap = 0;
		mLightType = directional;
	}
	virtual ~tzIGLLight() {}

protected:
	std::vector< std::shared_ptr<tzGLMesh> >				mMeshList;

	int					mShadowmapResolution;

	int					mGLModelMatrix;
	unsigned int		mGLShaderProgram;
	unsigned int		mGLLightDepthMapFBO;
	unsigned int		mGLLightShadowDepthMap;

	float				mNearPlane;
	float				mFarPlane;

	tzVector3D			mLightPos;

	tzLightType			mLightType;


public:
	virtual void		setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath, std::string geoShaderPath = "") = 0;
	virtual void		updateAttributes( ) = 0;
	unsigned int		shadowDepthMap( ) const
	{
		return mGLLightShadowDepthMap;
	}
	tzLightType			lightType( ) const
	{
		return mLightType;
	}
	void				setLightPos( const tzVector3D &lightPos )
	{
		mLightPos = lightPos;
	}
	tzVector3D			lightPos( ) const
	{
		return mLightPos;
	}
	

	virtual void							addMesh(std::shared_ptr<tzGLMesh> mesh)
	{
		mMeshList.push_back(mesh);
	}
	virtual const std::vector< std::shared_ptr<tzGLMesh> >&		meshList() const
	{
		return mMeshList;
	}

};

#endif