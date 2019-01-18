// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "tzMesh.h"


// ----------------------------------------------------------------  default constructor

tzMesh::tzMesh(void)
	: 	mNumVertices(0),
		mNumTriangles(0)	
{}


// ---------------------------------------------------------------- copy constructor
// this doesn't handle the vertex_faces

tzMesh::tzMesh(const tzMesh& m)
	: 	mVertices(m.mVertices),
		mNormals(m.mNormals),
		mUs(m.mUs),
		mVs(m.mVs),
		mNumVertices(m.mNumVertices),
		mNumTriangles(m.mNumTriangles)
{}


// ---------------------------------------------------------------- assignment operator
// this doesn't handle the vertex_faces

tzMesh&
tzMesh::operator= (const tzMesh& rhs) {
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


// ---------------------------------------------------------------- destructor

tzMesh::~tzMesh(void) {}


