#ifndef TZ_GL_MATERIAL_INTERFACE
#define TZ_GL_MATERIAL_INTERFACE

//#include "Common.h"
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include <vector>

#include "../glView/include/tzGLMesh.h"

class tzIGLMaterial
{
public:
	tzIGLMaterial( )
	{ 
	}
	virtual ~tzIGLMaterial() {}
protected:
	std::vector< tzGLMesh* >				mMeshList;

public:
	virtual void							setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath) = 0;
	virtual void							updateAttributes( const tzMatrix &modelMatrix ) = 0;
	virtual void							addMesh(tzGLMesh* mesh)
	{
		mMeshList.push_back(mesh);
	}
	virtual const std::vector< tzGLMesh* >&		meshList() const
	{
		return mMeshList;
	}
	virtual const std::map<std::string, tzCoreTexture*>& textureList( ) const = 0;
};

#endif