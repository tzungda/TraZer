#ifndef TZ_SCENE
#define TZ_SCENE

/*
This is for managing all types of the objects(mesh, implicit surface...etc) in a scene
*/

#include < vector >

#include "../Include/tzMesh.h"

using namespace std;

class tzScene
{
public:
	tzScene();
	virtual ~tzScene();
private:
	// all objects (including all types of objects)
	vector< tzObject* >		mObjectList;

	// mesh type
	vector< tzMesh* >		mMeshList;
	
	// implicit surface type
	// --- future work

public:
	

	void				addMesh( const tzMesh* newMesh );

	vector<tzMesh*>		meshList( );

};

#endif