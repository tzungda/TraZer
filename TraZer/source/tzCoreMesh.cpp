#include "../Include/tzCoreMesh.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreMesh::tzCoreMesh()
{
	mNumFaces = 0;
}

//===================================================================================
tzCoreMesh::~tzCoreMesh()
{
}


/*
Interfaces
*/

/*
-- set
*/

//===================================================================================
void tzCoreMesh::setNumFaces( unsigned int numFaces )
{
	mNumFaces = numFaces;
}

//===================================================================================
void tzCoreMesh::setPositions(const vector< float > &positions)
{
}

//===================================================================================
void tzCoreMesh::setIndices(const vector< unsigned int > &indices)
{
}

//===================================================================================
void tzCoreMesh::setVertexNormals(const vector< glm::vec3 > &vertexNormals)
{
}


/*
-- get
*/

//===================================================================================
const vector< float >& tzCoreMesh::floatPositions() const
{
	return mFloatPositions;
}

//===================================================================================
const vector< glm::vec3 >& tzCoreMesh::vertices() const
{
	return mVertices;
}

//===================================================================================
const unsigned int tzCoreMesh::numFaces() const
{
	return mNumFaces;
}

//===================================================================================
const vector< unsigned int >& tzCoreMesh::indices() const
{
	return this->mIndices;
}
