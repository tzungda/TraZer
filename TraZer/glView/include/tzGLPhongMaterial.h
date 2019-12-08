#ifndef TZ_GL_PHONG_MATERIAL_H
#define TZ_GL_PHONG_MATERIAL_H

/*
 Draw a line in the GL View
*/

#include "../../Include/tzCoreMaterial.h"



// (see http://www.fileformat.info/format/material/)
class tzGLPhongMaterial
{


public:
	tzGLPhongMaterial();
	virtual ~tzGLPhongMaterial( );

/*
my members
*/
private:
	tzCoreMaterial		*mPtrCoreMaterial;

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
	void					setMaterial( tzCoreMaterial *tzCoreMaterial);
	void					loadShader( const std::string& vertShaderPath, const std::string &fragShaderPath );
};

#endif
