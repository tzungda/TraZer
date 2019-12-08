#include "../Include/tzCoreTexture.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreTexture::tzCoreTexture()
{
}

//===================================================================================
tzCoreTexture::~tzCoreTexture()
{
}

//===================================================================================
void tzCoreTexture::setPath(const std::string &path)
{
	mPath = path;
}

//===================================================================================
const std::string& tzCoreTexture::path() const
{
	return mPath;
}

