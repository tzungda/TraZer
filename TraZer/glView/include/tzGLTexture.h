#ifndef TZ_GL_TEXTURE_H
#define TZ_GL_TEXTURE_H

/*
 Draw a line in the GL View
*/

#include "../../Include/tzCoreTexture.h"
#include "Interfaces/tzIGLObject.h"

//
class tzGLTexture : public tzIGLObject
{
public:
	tzGLTexture();
	virtual ~tzGLTexture( );

/*
my members
*/
private:
	tzTextureData		mTextureData;
	void				updateTextureData();
/*
my interfaces
*/
public:
	virtual void			setCoreObject(tzCoreObject *coreObjectPtr);
	const tzTextureData&	textureData( ) const;
};

#endif
