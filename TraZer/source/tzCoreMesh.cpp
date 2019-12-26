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
	mFloatVertices.resize(numVertices * 3);
	float *ptrP = &mFloatVertices[0];
	for ( int i = 0; i < numVertices; i++ )
	{
		(*ptrP) = vertices[i].x, ptrP++;
		(*ptrP) = vertices[i].y, ptrP++;
		(*ptrP) = vertices[i].z, ptrP++;
	}
}

//===================================================================================
void tzCoreMesh::setFloatVertices(const std::vector< float > &positions)
{
	mFloatVertices = positions;

	//
	mVertices.resize(positions.size() / 3);
	int idx = 0;
	for (int i = 0; i < (int)mVertices.size(); i++)
	{
		mVertices[i].x = mFloatVertices[idx], idx++;
		mVertices[i].y = mFloatVertices[idx], idx++;
		mVertices[i].z = mFloatVertices[idx], idx++;
	}
}

//===================================================================================
void tzCoreMesh::setVertexNormals(const std::vector< tzNormal > &normals)
{
	mVertexNormals = normals;
	//
	int numVertices = (int)normals.size();
	mFloatNormals.resize(numVertices * 3);
	float *ptrP = &mFloatNormals[0];
	for (int i = 0; i < numVertices; i++)
	{
		(*ptrP) = normals[i].x, ptrP++;
		(*ptrP) = normals[i].y, ptrP++;
		(*ptrP) = normals[i].z, ptrP++;
	}
}

//===================================================================================
void tzCoreMesh::setFloatNormals(const std::vector< float >& normals)
{
	mFloatNormals = normals;
	//
	mVertexNormals.resize(normals.size() / 3);
	int idx = 0;
	for (int i = 0; i < (int)mVertexNormals.size(); i++)
	{
		mVertexNormals[i].x = normals[idx], idx++;
		mVertexNormals[i].y = normals[idx], idx++;
		mVertexNormals[i].z = normals[idx], idx++;
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
void tzCoreMesh::setFloatTexcoords(const std::vector< float >& texcoord)
{
	this->mFloatTexcoords = texcoord;	
	//
	mU.resize(texcoord.size() / 2 );
	mV.resize(texcoord.size() / 2);
	int idx = 0;
	for (int i = 0; i < (int)texcoord.size(); i+=2)
	{
		mU[idx] = texcoord[i];
		mU[idx] = texcoord[i+1];
		idx++;
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

void tzCoreMesh::updateData( )
{
	bool uvsMoreThanVerts = false;
	bool vertsMoreThanUvs = false;
	if ( mU.size() != mVertices.size() )
	{
		if (mU.size() > mVertices.size())
		{
			uvsMoreThanVerts = true;
		}
		else
		{
			vertsMoreThanUvs = true;
		}
	}

	if (uvsMoreThanVerts)
	{
		mFloatVertices.resize(mU.size() * 3);
		mFloatNormals.resize(mU.size() * 3);
		mFloatTexcoords.resize(mU.size() * 2);

		for (int k = 0; k < (int)mIndices.size(); k++)
		{
			tzPoint3D vert = mVertices[ mIndices[k].vertex_index ];
			tzNormal norm = mVertexNormals[mIndices[k].normal_index];
			int uvid = mIndices[k].texcoord_index * 3;
			mFloatVertices[uvid] = vert.x;
			mFloatVertices[uvid + 1] = vert.y;
			mFloatVertices[uvid + 2] = vert.z;
			//
			mFloatNormals[uvid] = norm.x;
			mFloatNormals[uvid + 1] = norm.y;
			mFloatNormals[uvid + 2] = norm.z;
			//verts[shapes[i].mesh.indices[k].texcoord_index] = tzPoint3D(attrib.vertices[index], attrib.vertices[index + 1], attrib.vertices[index + 2]);
		}

		int uvId = 0;
		for (int i = 0; i < (int)mU.size(); i++)
		{
			mFloatTexcoords[uvId] = mU[i], uvId++;
			mFloatTexcoords[uvId] = mV[i], uvId++;
		}
	}
	else if (vertsMoreThanUvs)
	{
		mFloatVertices.resize(mVertices.size() * 3);
		int vId = 0;
		for (int i = 0; i < (int)mVertices.size(); i++)
		{
			tzPoint3D vert = mVertices[i];
			mFloatVertices[vId] = vert.x, vId++;
			mFloatVertices[vId] = vert.y, vId++;
			mFloatVertices[vId] = vert.z, vId++;
		}
		//
		mFloatNormals.resize(mVertexNormals.size() * 3);
		int nId = 0;
		for (int i = 0; i < (int)mVertexNormals.size(); i++)
		{
			tzNormal norm = mVertexNormals[i];
			mFloatNormals[nId] = norm.x, nId++;
			mFloatNormals[nId] = norm.y, nId++;
			mFloatNormals[nId] = norm.z, nId++;
		}

		//
		mFloatTexcoords.resize( mVertices.size()*2 );
		for (int k = 0; k < (int)mIndices.size(); k++)
		{
			float u = mU[mIndices[k].texcoord_index];
			float v = mV[mIndices[k].texcoord_index];

			int vid = mIndices[k].vertex_index * 2;
			mFloatTexcoords[vid] = u;
			mFloatTexcoords[vid+1] = v;
		}
	}
	else
	{
		mFloatVertices.resize( mVertices.size() * 3 );
		int vId = 0;
		for ( int i = 0; i < (int)mVertices.size(); i++ )
		{
			tzPoint3D vert = mVertices[i];
			mFloatVertices[vId] = vert.x, vId++;
			mFloatVertices[vId] = vert.y, vId++;
			mFloatVertices[vId] = vert.z, vId++;
		}
		//
		mFloatNormals.resize( mVertexNormals.size() * 3 );
		int nId = 0;
		for ( int i = 0; i < (int)mVertexNormals.size(); i++ )
		{
			tzNormal norm = mVertexNormals[i];
			mFloatNormals[nId] = norm.x, nId++;
			mFloatNormals[nId] = norm.y, nId++;
			mFloatNormals[nId] = norm.z, nId++;
		}
		//
		mFloatTexcoords.resize(mU.size()*2);
		int uvId = 0;
		for ( int i = 0; i < (int)mU.size(); i++ )
		{
			mFloatTexcoords[uvId] = mU[i], uvId++;
			mFloatTexcoords[uvId] = mV[i], uvId++;
		}
	}
	/*
	mFloatVertices.resize( mIndices.size() * 3 );
	mFloatNormals.resize( mIndices.size() * 3 );
	mFloatTexcoords.resize( mIndices.size()*2 );

	int vId = 0, nId = 0, uvId = 0;
	for ( int i = 0; i < (int)mIndices.size(); i++ )
	{
		tzPoint3D vert = mVertices[mIndices[i].vertex_index];
		mFloatVertices[vId] = vert.x, vId++;
		mFloatVertices[vId] = vert.y, vId++;
		mFloatVertices[vId] = vert.z, vId++;

		tzNormal norm = mVertexNormals[mIndices[i].normal_index];
		mFloatNormals[nId] = norm.x, nId++;
		mFloatNormals[nId] = norm.y, nId++;
		mFloatNormals[nId] = norm.z, nId++;

		mFloatTexcoords[uvId] = mU[mIndices[i].texcoord_index], uvId++;
		mFloatTexcoords[uvId] = mV[mIndices[i].texcoord_index], uvId++;
	}
	*/

}

//===================================================================================
const std::vector< float >& tzCoreMesh::floatVertices() const
{
	return mFloatVertices;
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
const std::vector< float >& tzCoreMesh::floatTexcoords()
{
	return mFloatTexcoords;
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
const void	tzCoreMesh::indices(std::vector<unsigned int> &idx) const
{
	if ( mU.size() > mVertices.size() )
	{
		idx.resize(mIndices.size());
		for ( int j = 0; j < (int)mIndices.size(); j++ )
		{
			idx[j] = mIndices[j].texcoord_index;
		}
	}
	else
	{
		idx.resize(mIndices.size());
		for (int j = 0; j < (int)mIndices.size(); j++)
		{
			idx[j] = mIndices[j].vertex_index;
		}
	}
	/*
	for ( int i = 0; i < (int)mFaceVertices.size(); i++ )
	{
		for ( int j = 0; j < (int)mFaceVertices[i].size(); j++ )
		{
			idx.push_back( mFaceVertices[i][j] );
		}
	}
	*/
	
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

