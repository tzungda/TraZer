#ifndef TZ_GL_TEXTURE_H
#define TZ_GL_TEXTURE_H

/*
 Draw a line in the GL View
*/

#include "../../Include/tzCoreTexture.h"


//
class tzGLTexture
{
public:
	tzGLTexture();
	virtual ~tzGLTexture( );

/*
my members
*/
private:
	tzCoreTexture		*mPtrCoreTexture;
	tzTextureData		mTextureData;

	void				updateTextureData();
/*
my interfaces
*/
public:
	void					setTexture( tzCoreTexture *tzCoreTexture);
	const tzCoreTexture*	texture( ) const;
	const tzTextureData&	textureData( ) const;
};

#endif
