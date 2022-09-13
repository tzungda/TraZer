#include "../Include/tzCoreScene.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreScene::tzCoreScene()
{
}

//===================================================================================
tzCoreScene::~tzCoreScene()
{
	mObjectList.clear();
}


/*
Interfaces
*/

//===================================================================================
void tzCoreScene::addMesh(const std::shared_ptr<tzCoreMesh> newMesh)
{
	if ( newMesh == nullptr )
	{
		return;
	}
	mObjectList.push_back( (std::shared_ptr<tzCoreObject>)newMesh );
	mMeshList.push_back( newMesh );
}

//===================================================================================
const std::vector<std::shared_ptr<tzCoreMesh>>& tzCoreScene::meshList() const
{
	return mMeshList;
}

//===================================================================================
void tzCoreScene::addMaterial( const std::shared_ptr<tzCoreMaterial> newMaterial )
{
	if ( newMaterial == nullptr )
	{
		return;
	}

	mObjectList.push_back( (std::shared_ptr<tzCoreObject>)newMaterial );
	mMaterialList.push_back( newMaterial );
}

//===================================================================================
const std::vector<std::shared_ptr<tzCoreMaterial>>& tzCoreScene::materialList( ) const
{
	return mMaterialList;
}

//===================================================================================
void tzCoreScene::addTexture( std::shared_ptr<tzCoreTexture> newTexture)
{
	if ( newTexture == nullptr)
	{
		return;
	}
	this->mTextureList[newTexture->path()] = newTexture;
	mObjectList.push_back( (std::shared_ptr<tzCoreObject>)newTexture);
}

//===================================================================================
const std::map< std::string, std::shared_ptr<tzCoreTexture> >& tzCoreScene::textureList() const
{
	return this->mTextureList;
}

//===================================================================================
void tzCoreScene::addLight(const std::shared_ptr<tzCoreLight> newLight)
{
	if ( newLight == nullptr)
	{
		return;
	}
	mLightList.push_back(newLight );
	mObjectList.push_back((std::shared_ptr<tzCoreObject>)newLight);
}

//===================================================================================
const std::vector<std::shared_ptr<tzCoreLight>>& tzCoreScene::lightList() const
{
	return mLightList;
}

