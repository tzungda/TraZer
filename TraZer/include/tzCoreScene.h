#ifndef TZ_SCENE
#define TZ_SCENE

/*
This is for managing all types of the objects(mesh, implicit surface...etc) in a scene
*/

#include < vector >
#include <memory>

// core headers
#include "../Include/tzCoreMesh.h"
#include "../Include/tzCoreMaterial.h"
#include "../Include/tzCoreLight.h"


class tzCoreScene
{
public:
	tzCoreScene();
	virtual ~tzCoreScene();

private:
	// all objects (including all types of objects)
	std::vector< std::shared_ptr< tzCoreObject > >		mObjectList;

	// mesh list
	std::vector<std::shared_ptr<tzCoreMesh> >		mMeshList;
	
	// material list
	std::vector< std::shared_ptr<tzCoreMaterial> >	mMaterialList;

	// light list
	std::vector< std::shared_ptr<tzCoreLight> >	mLightList;

	// texture list
	std::map< std::string, std::shared_ptr<tzCoreTexture> >			mTextureList; // < image path, textureObject >

public:
	
	void																addMesh( const std::shared_ptr<tzCoreMesh> newMesh );
	const std::vector<std::shared_ptr<tzCoreMesh>>&						meshList( ) const;

	void																addMaterial( const std::shared_ptr<tzCoreMaterial> newMaterial );
	const std::vector<std::shared_ptr<tzCoreMaterial>>&					materialList( ) const;

	void																addTexture( std::shared_ptr<tzCoreTexture> newTexture );
	const std::map< std::string, std::shared_ptr<tzCoreTexture> >&		textureList( ) const;

	void																addLight( const std::shared_ptr<tzCoreLight> newLight );
	const std::vector<std::shared_ptr<tzCoreLight>>&					lightList( ) const;
};

#endif