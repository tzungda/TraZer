#include "../Include/tzMesh.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzMesh::tzMesh()
{
	mNumFaces = 0;
}

//===================================================================================
tzMesh::~tzMesh()
{
}


/*
Interfaces
*/

/*
-- set
*/

//===================================================================================
void tzMesh::setNumFaces( unsigned int numFaces )
{
	mNumFaces = numFaces;
}

//===================================================================================
void tzMesh::setPositions(const vector< float > &positions)
{
}

//===================================================================================
void tzMesh::setIndices(const vector< unsigned int > &indices)
{
}

//===================================================================================
void tzMesh::setVertexNormals(const vector< glm::vec3 > &vertexNormals)
{
}


/*
-- get
*/

//===================================================================================
const vector< float >& tzMesh::floatPositions() const
{
	return mFloatPositions;
}

//===================================================================================
const vector< glm::vec3 >& tzMesh::vertices() const
{
	return mVertices;
}

//===================================================================================
const unsigned int tzMesh::numFaces() const
{
	return mNumFaces;
}

//===================================================================================
const vector< unsigned int >& tzMesh::indices() const
{
	return this->mIndices;
}
