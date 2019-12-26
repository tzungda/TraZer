#include "../include/tzGLTexture.h"
#include "../include/tzTool.h"

/*
Constructor/Destructor
*/

//================================================================================
tzGLTexture::tzGLTexture()
{
}

//================================================================================
tzGLTexture::~tzGLTexture()
{
}

//================================================================================
void tzGLTexture::updateTextureData()
{
	if ( !mPtrCoreObject )
	{
		printf( " Texture isn't available \n" );
		return;
	}

	mTextureData = tzTool::loadPngTexture( ((tzCoreTexture*)mPtrCoreObject)->path().c_str());
}

//================================================================================
void tzGLTexture::setCoreObject(tzCoreObject *coreObjectPtr)
{
	mPtrCoreObject = coreObjectPtr;
	updateTextureData();
}


//================================================================================
const tzTextureData& tzGLTexture::textureData() const
{
	return mTextureData;
}
