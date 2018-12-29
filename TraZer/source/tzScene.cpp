#include "../Include/tzScene.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzScene::tzScene()
{
}

//===================================================================================
tzScene::~tzScene()
{
	mObjectList.clear();
}


/*
Interfaces
*/

//===================================================================================
void tzScene::addMesh(const tzMesh* newMesh)
{
	mObjectList.push_back( (tzMesh*)newMesh );
	mMeshList.push_back( (tzMesh*)newMesh );
}

//===================================================================================
vector<tzMesh*> tzScene::meshList()
{
	return mMeshList;
}