#include "../Include/tzCoreCamera.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreCamera::tzCoreCamera()
{
	mNumVertices = mNumTriangles = 0;
}

//===================================================================================
tzCoreCamera::~tzCoreCamera()
{
}


/*
Interfaces
*/

/*
-- set
*/

//===================================================================================
void tzCoreCamera::setVertices(const vector< tzPoint3D > &vertices)
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
void tzCoreCamera::setVertices(const vector< float > &positions)
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
void tzCoreCamera::setVertexNormals(const vector< tzNormal > &normals)
{
	mVertexNormals = normals;
}

//===================================================================================
void tzCoreCamera::setUs(const vector< float > &us)
{
	mU = us;
}

//===================================================================================
void tzCoreCamera::setVs(const vector< float > &vs)
{
	mV = vs;
}

//===================================================================================
void tzCoreCamera::setVertexFaces(const vector<vector<int> > vertexFaces)
{
	mVertexFaces = vertexFaces;
}

//===================================================================================
void tzCoreCamera::setNumVertices(int numVertices)
{
	mNumVertices = numVertices;
}

//===================================================================================
void tzCoreCamera::setNumTriangles(int numTriangles)
{
	mNumTriangles = numTriangles;
}

//===================================================================================
void tzCoreCamera::setIndices(const vector< tzCoreCamera::index > &indices)
{
	mIndices = indices;
}

//===================================================================================
void tzCoreCamera::setFaceVertices(const vector<vector<int>> &faceVertices)
{
	mFaceVertices = faceVertices;
}

/*
-- get
*/

//===================================================================================
const vector< float >& tzCoreCamera::floatPositions() const
{
	return mPositions;
}

//===================================================================================
const vector< tzPoint3D >& tzCoreCamera::vertices() const
{
	return mVertices;
}

//===================================================================================
const vector< tzNormal >& tzCoreCamera::vertexNormals() const
{
	return mVertexNormals;
}

//===================================================================================
const vector< float >& tzCoreCamera::us() const
{
	return mU;
}

//===================================================================================
const vector< float >& tzCoreCamera::vs() const
{
	return mV;
}

//===================================================================================
const vector<vector<int> > tzCoreCamera::vertexFaces() const
{
	return mVertexFaces;
}

//===================================================================================
int tzCoreCamera::numVertices() const
{
	return mNumVertices;
}

//===================================================================================
int tzCoreCamera::numTriangles() const
{
	return mNumTriangles;
}

//===================================================================================
const vector< tzCoreCamera::index >& tzCoreCamera::indices() const
{
	return mIndices;
}

//===================================================================================
const vector<vector<int>>& tzCoreCamera::faceVertices() const
{
	return mFaceVertices;
}
