
#include "../include/tzCoreMaterial.h"

/*
constructor/destructor
*/

//===================================================================================
tzCoreMaterial::tzCoreMaterial()
{
}

//===================================================================================
tzCoreMaterial::~tzCoreMaterial()
{
}


/*
interfaces
*/

//===================================================================================
tzVector3D tzCoreMaterial::ambientColor() const
{
	return mAmbientColor;
}

//===================================================================================
tzVector3D tzCoreMaterial::diffuseColor() const
{
	return mDiffuseColor;
}

//===================================================================================
tzVector3D tzCoreMaterial::specularColor() const
{
	return mSpecularColor;
}

//===================================================================================
void tzCoreMaterial::setAmbientColor(const tzVector3D& color)
{
	mAmbientColor = color;
}

//===================================================================================
void tzCoreMaterial::setDiffuseColor(const tzVector3D& color)
{
	mDiffuseColor = color;
}

//===================================================================================
void tzCoreMaterial::setSpecularColor(const tzVector3D& color)
{
	mSpecularColor = color;
}

//===================================================================================
void tzCoreMaterial::addTexture(const std::string &name, std::shared_ptr<tzCoreTexture> texture)
{
	mTextureList[name] = texture;
}

//===================================================================================
void tzCoreMaterial::addAttribute(const std::string &name, tzVector3D value)
{
	mAttributeList[name] = value;
}

//===================================================================================
void tzCoreMaterial::addFloatAttribute(const std::string &name, float value)
{
	mFloatAttributeList[name] = value;
}

//===================================================================================
const std::map<std::string, std::shared_ptr<tzCoreTexture>>& tzCoreMaterial::textureList() const
{
	return this->mTextureList;
}

//===================================================================================
const std::map<std::string, tzVector3D>& tzCoreMaterial::attributeList() const
{
	return this->mAttributeList;
}

//===================================================================================
const std::map<std::string, float>& tzCoreMaterial::floatAttributeList() const
{
	return this->mFloatAttributeList;
}

