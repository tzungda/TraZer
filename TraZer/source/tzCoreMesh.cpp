#include "../Include/tzCoreMesh.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreMesh::tzCoreMesh()
{
	mNumVertices = mNumTriangles = 0;
	mPtrCoreMaterial = NULL;
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
void tzCoreMesh::setVertices(const std::vector< tzPoint3D > &vertices)
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
void tzCoreMesh::setVertices(const std::vector< float > &positions)
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
void tzCoreMesh::setVertexNormals(const std::vector< tzNormal > &normals)
{
	mVertexNormals = normals;
}

//===================================================================================
void tzCoreMesh::updateFloatNormals(const std::vector< float >* normals)
{
	if(normals)
	{
		mFloatNormals = (*normals);
	}
	else
	{
		if ( mVertexNormals.size( ) == 0 )
		{
			printf( "Can't find the vertex normal \n" );
			return;
		}

		mFloatNormals.resize(mVertexNormals.size() * 3);
		float *ptr = &mFloatNormals[0];
		for ( int i = 0; i < mVertexNormals.size(); i++ )
		{
			(*ptr++) = mVertexNormals[i].x;
			(*ptr++) = mVertexNormals[i].y;
			(*ptr++) = mVertexNormals[i].z;
		}
	}
}

//===================================================================================
void tzCoreMesh::setUs(const std::vector< float > &us)
{
	mU = us;
}

//===================================================================================
void tzCoreMesh::setVs(const std::vector< float > &vs)
{
	mV = vs;
}

//===================================================================================
void tzCoreMesh::setMaterialIds(const std::vector<int> &matIds)
{
	this->mMaterialIds = matIds;
}

//===================================================================================
void tzCoreMesh::updateTexcoords(const std::vector< float >* texcoord)
{
	if ( texcoord )
	{
		this->mTexcoords = (*texcoord);
	}
	else
	{
		if( (mU.size() > 0 || mV.size() > 0 ) && mU.size() != mV.size() )
		{
			printf( "size of u doesn't match to size of v\n" );
			return;
		}
		mTexcoords.resize(mU.size() * 2 );
		float *ptr = &mTexcoords[0];
		for ( int i = 0; i < mU.size(); i++ )
		{
			(*ptr++) = mU[i];
			(*ptr++) = mV[i];
		}
	}
}

//===================================================================================
void tzCoreMesh::setVertexFaces(const std::vector<std::vector<int> > vertexFaces)
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
void tzCoreMesh::setIndices(const std::vector< tzCoreMesh::index > &indices)
{
	mIndices = indices;
}

//===================================================================================
void tzCoreMesh::setFaceVertices(const std::vector<std::vector<int>> &faceVertices)
{
	mFaceVertices = faceVertices;
}

//===================================================================================
void tzCoreMesh::setMaterial(tzCoreMaterial *mat)
{
	this->mPtrCoreMaterial = mat;
}

/*
-- get
*/

//===================================================================================
const std::vector< float >& tzCoreMesh::floatPositions() const
{
	return mPositions;
}

//===================================================================================
const std::vector< tzPoint3D >& tzCoreMesh::vertices() const
{
	return mVertices;
}

//===================================================================================
const std::vector< tzNormal >& tzCoreMesh::vertexNormals() const
{
	return mVertexNormals;
}

//===================================================================================
const std::vector< float >& tzCoreMesh::floatNormals()
{
	if ( mFloatNormals.size() == 0 )
	{
		updateFloatNormals();
	}
	return mFloatNormals;
}

//===================================================================================
const std::vector< float >& tzCoreMesh::us() const
{
	return mU;
}

//===================================================================================
const std::vector< float >& tzCoreMesh::vs() const
{
	return mV;
}

//===================================================================================
const std::vector< float >& tzCoreMesh::texcoords() 
{
	if ( mTexcoords.size() == 0 )
	{
		updateTexcoords();
	}
	return mTexcoords;
}

//===================================================================================
const std::vector<std::vector<int> > tzCoreMesh::vertexFaces() const
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
const std::vector< tzCoreMesh::index >& tzCoreMesh::indices() const
{
	return mIndices;
}

//===================================================================================
const std::vector<std::vector<int>>& tzCoreMesh::faceVertices() const
{
	return mFaceVertices;
}

//===================================================================================
const std::vector<int>& tzCoreMesh::materialIds() const
{
	return this->mMaterialIds;
}

//===================================================================================
tzCoreMaterial* tzCoreMesh::material() const
{
	return mPtrCoreMaterial;
}

