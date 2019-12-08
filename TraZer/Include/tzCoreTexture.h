#ifndef TZ_CORETEXTURE
#define TZ_CORETEXTURE

/*
This is for managing all types of the objects(mesh, implicit surface...etc) in a scene
*/

#include "tzCoreObject.h"
#include < string >

// core headers

typedef struct _tzTextureData
{
	_tzTextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} tzTextureData;

class tzCoreTexture: public tzCoreObject
{
public:
	tzCoreTexture();
	virtual ~tzCoreTexture();

private:
	std::string			mPath;

public:

	void				setPath( const std::string &path );

	const std::string&	path( ) const;

};

#endif