#include "../include/tzGLTexture.h"
#include "../include/tzImageTool.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLTexture::tzGLTexture()
{
	mPtrCoreTexture = NULL;
}

//================================================================================
tzGLTexture::~tzGLTexture()
{
}

//================================================================================
void tzGLTexture::updateTextureData()
{
	if ( !mPtrCoreTexture )
	{
		printf( " Texture isn't available \n" );
		return;
	}

	mTextureData = tzImageTool::LoadPngTexture(mPtrCoreTexture->path().c_str());
}

//================================================================================
void tzGLTexture::setTexture( tzCoreTexture *tzCoreTexture)
{
	mPtrCoreTexture = tzCoreTexture;
}

//================================================================================
const tzCoreTexture* tzGLTexture::texture() const
{
	return mPtrCoreTexture;
}

//================================================================================
const tzTextureData& tzGLTexture::textureData() const
{
	return mTextureData;
}
