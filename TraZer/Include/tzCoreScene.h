#ifndef TZ_SCENE
#define TZ_SCENE

/*
This is for managing all types of the objects(mesh, implicit surface...etc) in a scene
*/

#include < vector >

// core headers
#include "../Include/tzCoreMesh.h"

// gl headers
#include "../glView/include/tzGLMesh.h"

using namespace std;

class tzCoreScene
{
public:
	tzCoreScene();
	virtual ~tzCoreScene();
private:
	// all objects (including all types of objects)
	vector< tzCoreObject* >		mObjectList;

	// mesh type
	vector< tzCoreMesh* >		mMeshList;
	
	// implicit surface type
	// --- future work

public:
	

	void				addMesh( const tzCoreMesh* newMesh );

	vector<tzCoreMesh*>		meshList( );

};

#endif