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
	mObjectList.push_back( (tzCoreMesh*)newMesh );
	mMeshList.push_back( (tzCoreMesh*)newMesh );
}

//===================================================================================
vector<tzCoreMesh*> tzCoreScene::meshList()
{
	return mMeshList;
}