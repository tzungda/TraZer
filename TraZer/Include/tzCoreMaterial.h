#ifndef TZ_CORE_MATERIAL
#define TZ_CORE_MATERIAL


#include <map>
#include <string>
#include "../include/tzCoreTexture.h"
#include "../Utilities/tzVector3D.h"


class tzCoreMaterial: public tzCoreObject
{
private:
	tzVector3D									mAmbientColor;
	tzVector3D									mDiffuseColor;
	tzVector3D									mSpecularColor;

	std::map<std::string, tzCoreTexture*>		mTextureList; // attribute(eg. diffuse, ambient) and the correspond texture
	std::map<std::string, tzVector3D>			mAttributeList; // additional attributes other than the default diffuse, specular and ambient
	std::map<std::string, float>				mFloatAttributeList; // additional attributes other than the default diffuse, specular and ambient

public:
	/*
	constructor/destructor
	*/
	tzCoreMaterial();
	virtual ~tzCoreMaterial();

public:
	/*
	interfaces
	*/
	tzVector3D										ambientColor( ) const;
	tzVector3D										diffuseColor() const;
	tzVector3D										specularColor() const;
	void											setAmbientColor(const tzVector3D& color );
	void											setDiffuseColor(const tzVector3D& color);
	void											setSpecularColor(const tzVector3D& color);
	void											addTexture( const std::string &name, tzCoreTexture* texture );
	void											addAttribute( const std::string &name, tzVector3D value );
	void											addFloatAttribute(const std::string &name, float value);
	const std::map<std::string, tzCoreTexture*>&	textureList( ) const;
	const std::map<std::string, tzVector3D>&		attributeList( ) const;
	const std::map<std::string, float>&				floatAttributeList( ) const;
} ;

#endif
