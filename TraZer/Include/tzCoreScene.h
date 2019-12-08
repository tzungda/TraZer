#ifndef TZ_SCENE
#define TZ_SCENE

/*
This is for managing all types of the objects(mesh, implicit surface...etc) in a scene
*/

#include < vector >

// core headers
#include "../Include/tzCoreMesh.h"
#include "../Include/tzCoreMaterial.h"



class tzCoreScene
{
public:
	tzCoreScene();
	virtual ~tzCoreScene();

private:
	// all objects (including all types of objects)
	std::vector< tzCoreObject* >		mObjectList;

	// mesh list
	std::vector< tzCoreMesh* >		mMeshList;
	
	// material list
	std::vector< tzCoreMaterial* >	mMaterialList;

	// texture list
	std::map< std::string, tzCoreTexture* >			mTextureList; // < image path, textureObject >

public:
	
	void												addMesh( const tzCoreMesh* newMesh );
	const std::vector<tzCoreMesh*>&						meshList( ) const;

	void												addMaterial( const tzCoreMaterial* newMaterial );
	const std::vector<tzCoreMaterial*>&					materialList( ) const;

	void												addTexture( tzCoreTexture* newTexture );
	const std::map< std::string, tzCoreTexture* >&		textureList( ) const;
};

#endif