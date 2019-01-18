#ifndef __MESH__
#define __MESH__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// A mesh object stores the data for mesh triangles:
//
//			vertices
//			normals
//			uv texture coordinates
//			lists of triangles that share each vertex
//
// Mesh triangles are stored in a grid, which has a pointer to the mesh
// Each mesh triangle also has a pointer to the mesh
// The Mesh class does not inherit from tzIGeometricObject

#include <vector>

#include "tzPoint3D.h"
#include "tzNormal.h"

using namespace std;
	
class tzMesh {
	public:

		vector<tzPoint3D> 		mVertices;				// mesh vertices 
		vector<tzNormal>		mNormals;				// average normal at each vertex;
		vector<float>			mUs;						// u texture coordinate at each vertex
		vector<float>			mVs;						// v texture coordinate at each vertex
		vector<vector<int> > 	mVertexFaces;			// the triangles shared by each vertex
		int 					mNumVertices; 			// number of vertices
		int 					mNumTriangles; 			// number of triangles
		
		tzMesh(void);
			
		tzMesh(const tzMesh& m);

		~tzMesh(void);

		tzMesh&
		operator= (const tzMesh& rhs);
};

#endif


