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
void tzCoreScene::addMesh(const tzCoreMesh* newMesh)
{
	if ( ((tzCoreMesh*)newMesh) == nullptr )
	{
		return;
	}
	mObjectList.push_back( (tzCoreObject*)newMesh );
	mMeshList.push_back( (tzCoreMesh*)newMesh );
}

//===================================================================================
const std::vector<tzCoreMesh*>& tzCoreScene::meshList() const
{
	return mMeshList;
}

//===================================================================================
void tzCoreScene::addMaterial( const tzCoreMaterial* newMaterial )
{
	if ( ((tzCoreMaterial*)newMaterial) == nullptr )
	{
		return;
	}

	mObjectList.push_back( (tzCoreObject*)newMaterial );
	mMaterialList.push_back( (tzCoreMaterial*)newMaterial );
}

//===================================================================================
const std::vector<tzCoreMaterial*>& tzCoreScene::materialList( ) const
{
	return mMaterialList;
}

//===================================================================================
void tzCoreScene::addTexture( tzCoreTexture* newTexture)
{
	if (((tzCoreTexture*)newTexture) == nullptr)
	{
		return;
	}
	this->mTextureList[newTexture->path()] = newTexture;
	mObjectList.push_back((tzCoreObject*)newTexture);
}

//===================================================================================
const std::map< std::string, tzCoreTexture* >& tzCoreScene::textureList() const
{
	return this->mTextureList;
}

//===================================================================================
void tzCoreScene::addLight(const tzCoreLight* newLight)
{
	if (((tzCoreLight*)newLight) == nullptr)
	{
		return;
	}
	mLightList.push_back((tzCoreLight*)newLight );
	mObjectList.push_back((tzCoreObject*)newLight);
}

//===================================================================================
const std::vector<tzCoreLight*>& tzCoreScene::lightList() const
{
	return mLightList;
}

