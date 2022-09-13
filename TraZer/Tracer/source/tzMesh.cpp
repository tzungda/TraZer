
#include "tzMesh.h"

//===================================================================================
tzMesh::tzMesh(void)
	: 	mNumVertices(0),
		mNumTriangles(0)	
{}

//===================================================================================
tzMesh::tzMesh(const tzMesh& m)
	: 	mVertices(m.mVertices),
		mNormals(m.mNormals),
		mUs(m.mUs),
		mVs(m.mVs),
		mNumVertices(m.mNumVertices),
		mNumTriangles(m.mNumTriangles)
{}

//===================================================================================
tzMesh& tzMesh::operator= (const tzMesh& rhs) 
{
	if (this == &rhs)
		return (*this);

	mVertices = rhs.mVertices;
	mNormals = rhs.mNormals;
	mUs = rhs.mUs;
	mVs = rhs.mVs;
	mNumVertices = rhs.mNumVertices;
	mNumTriangles = rhs.mNumTriangles;
	
	return (*this);
}

//===================================================================================
tzMesh::~tzMesh(void) 
{
}


