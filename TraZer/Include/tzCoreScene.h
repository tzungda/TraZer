#ifndef TZ_SCENE
#define TZ_SCENE

/*
This is for managing all types of the objects(mesh, implicit surface...etc) in a scene
*/

#include < vector >

// core headers
#include "../Include/tzCoreMesh.h"
#include "../Include/tzCoreMaterial.h"

// gl headers
#include "../glView/include/tzGLMesh.h"

using namespace std;

class tzCoreScene
{
public:
	tzCoreScene();
	virtual ~tzCoreScene();

	// texture list
	vector< string >			mTexturePathList;

private:
	// all objects (including all types of objects)
	vector< tzCoreObject* >		mObjectList;

	// mesh list
	vector< tzCoreMesh* >		mMeshList;
	
	// material list
	vector< tzCoreMaterial* >	mMaterialList;

public:
	
	void						addMesh( const tzCoreMesh* newMesh );
	vector<tzCoreMesh*>			meshList( );

	void						addMaterial( const tzCoreMaterial* newMaterial );
	vector<tzCoreMaterial*>		materialList( );

};

#endif