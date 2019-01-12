#include "../Include/tzCoreMesh.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreMesh::tzCoreMesh()
{
	mNumVertices = mNumTriangles = 0;
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
void tzCoreMesh::setVertices(const vector< tzPoint3D > &vertices)
{
	int numVertices = (int)vertices.size();
	if (numVertices == 0 )
	{
		printf(" the length of the vertices is 0 \n");
		return;
	}
	mVertices = vertices;

	//
	mPositions.resize(numVertices * 3);
	float *ptrP = &mPositions[0];
	for ( int i = 0; i < numVertices; i++ )
	{
		(*ptrP) = vertices[i].x, ptrP++;
		(*ptrP) = vertices[i].y, ptrP++;
		(*ptrP) = vertices[i].z, ptrP++;
	}
}

//===================================================================================
void tzCoreMesh::setVertices(const vector< float > &positions)
{
	int numPositions = (int)positions.size();
	if (numPositions == 0 || numPositions % 3 != 0)
	{
		printf(" the length of the positions needs to be a multiple of 3 \n");
		return;
	}
	mPositions = positions;

	//
	mVertices.resize(numPositions / 3);
	int idx = 0;
	for (int i = 0; i < (int)mVertices.size(); i++)
	{
		mVertices[i].x = mPositions[idx], idx++;
		mVertices[i].y = mPositions[idx], idx++;
		mVertices[i].z = mPositions[idx], idx++;
	}
}

//===================================================================================
void tzCoreMesh::setVertexNormals(const vector< tzNormal > &normals)
{
	mVertexNormals = normals;
}

//===================================================================================
void tzCoreMesh::setUs(const vector< float > &us)
{
	mU = us;
}

//===================================================================================
void tzCoreMesh::setVs(const vector< float > &vs)
{
	mV = vs;
}

//===================================================================================
void tzCoreMesh::setVertexFaces(const vector<vector<int> > vertexFaces)
{
	mVertexFaces = vertexFaces;
}

//===================================================================================
void tzCoreMesh::setNumVertices(int numVertices)
{
	mNumVertices = numVertices;
}

//===================================================================================
void tzCoreMesh::setNumTriangles(int numTriangles)
{
	mNumTriangles = numTriangles;
}

//===================================================================================
void tzCoreMesh::setIndices(const vector< tzCoreMesh::index > &indices)
{
	mIndices = indices;
}

//===================================================================================
void tzCoreMesh::setFaceVertices(const vector<vector<int>> &faceVertices)
{
	mFaceVertices = faceVertices;
}

/*
-- get
*/

//===================================================================================
const vector< float >& tzCoreMesh::floatPositions() const
{
	return mPositions;
}

//===================================================================================
const vector< tzPoint3D >& tzCoreMesh::vertices() const
{
	return mVertices;
}

//===================================================================================
const vector< tzNormal >& tzCoreMesh::vertexNormals() const
{
	return mVertexNormals;
}

//===================================================================================
const vector< float >& tzCoreMesh::us() const
{
	return mU;
}

//===================================================================================
const vector< float >& tzCoreMesh::vs() const
{
	return mV;
}

//===================================================================================
const vector<vector<int> > tzCoreMesh::vertexFaces() const
{
	return mVertexFaces;
}

//===================================================================================
int tzCoreMesh::numVertices() const
{
	return mNumVertices;
}

//===================================================================================
int tzCoreMesh::numTriangles() const
{
	return mNumTriangles;
}

//===================================================================================
const vector< tzCoreMesh::index >& tzCoreMesh::indices() const
{
	return mIndices;
}

//===================================================================================
const vector<vector<int>>& tzCoreMesh::faceVertices() const
{
	return mFaceVertices;
}
