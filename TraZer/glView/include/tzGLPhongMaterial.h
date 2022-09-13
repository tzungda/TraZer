#ifndef TZ_GL_PHONG_MATERIAL_H
#define TZ_GL_PHONG_MATERIAL_H

/*
 Draw a line in the GL View
*/

#include "../../Include/tzCoreMaterial.h"
#include "Interfaces/tzIGLObject.h"
#include "Interfaces/tzIGLMaterial.h"


// (see http://www.fileformat.info/format/material/)
class tzGLPhongMaterial : public tzIGLObject,
						  public tzIGLMaterial
{

public:
	tzGLPhongMaterial();
	virtual ~tzGLPhongMaterial( );

/*
my members
*/

public:
	std::string			mName;
	tzVector3D			mAmbient;
	tzVector3D			mDiffuse;
	tzVector3D			mSpecular;
	tzVector3D			mTransmittance;
	tzVector3D			mEmission;
	float				mShininess;
	float				mIor;
	float				mDissolve;
	int					mIllum;

	int mGLModelMatrix;
	int mGLViewMatrix;
	int mGLProjectionMatrix;
	int mGLLightMatrix;
	int mGLLightPos;
	int mGLViewPos;
	int mGLShininess;

	int mGLAmbient;
	int mGLDiffuse;
	int mGLSpecular;

	unsigned int		mGLTexAmbient;
	unsigned int		mGLTexDiffuse;
	unsigned int		mGLTexSpecular;
	unsigned int		mGLTexSpecularHighlight;
	unsigned int		mGLTexBump;
	unsigned int		mGLTexAlpha;
	int mGLHasAmbientTex;
	int mGLHasDiffuseTex;
	int mGLHasSpecularTex;

	unsigned char		mHasAmbientTex;
	std::string			mAmbientTexPath;
	unsigned char		mHasDiffuseTex;
	std::string			mDiffuseTexPath;
	unsigned char		mHasSpecularTex;
	std::string			mSpecularTexPath;

	//
	unsigned int		mGLPhongShaderProgram;

	//
	void				updateMaterial();

/*
my interfaces
*/
public:
	

/*
derived interfaces
*/
public:
	// tzIGLObject interface
	virtual	void			setCoreObject(std::shared_ptr<tzCoreObject> coreObjectPtr);

	// tzIGLMaterial interface
	virtual void			setupShaders(const std::string& vertShaderPath, const std::string &fragShaderPath);
	virtual void			updateAttributes(const tzMatrix &modelMatrix);
	const std::map<std::string, std::shared_ptr<tzCoreTexture>>& textureList() const;

};

#endif
