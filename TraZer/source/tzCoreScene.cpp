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
	mObjectList.push_back( (tzCoreMesh*)newMesh );
	mMeshList.push_back( (tzCoreMesh*)newMesh );
}

//===================================================================================
vector<tzCoreMesh*> tzCoreScene::meshList()
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

	mObjectList.push_back( (tzCoreMaterial*)newMaterial );
	mMaterialList.push_back( (tzCoreMaterial*)newMaterial );
}

//===================================================================================
vector<tzCoreMaterial*> tzCoreScene::materialList( )
{
	return mMaterialList;
}


