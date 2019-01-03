// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "tzMesh.h"


// ----------------------------------------------------------------  default constructor

tzMesh::tzMesh(void)
	: 	num_vertices(0),
		num_triangles(0)	
{}


// ---------------------------------------------------------------- copy constructor
// this doesn't handle the vertex_faces

tzMesh::tzMesh(const tzMesh& m)
	: 	vertices(m.vertices),
		normals(m.normals),
		u(m.u),
		v(m.v),
		num_vertices(m.num_vertices),
		num_triangles(m.num_triangles)
{}


// ---------------------------------------------------------------- assignment operator
// this doesn't handle the vertex_faces

tzMesh&
tzMesh::operator= (const tzMesh& rhs) {
	if (this == &rhs)
		return (*this);

	vertices 		= rhs.vertices;  
	normals  		= rhs.normals;
	u  				= rhs.u;
	v  				= rhs.v;
	num_vertices	= rhs.num_vertices;
	num_triangles	= rhs.num_triangles;
	
	return (*this);
}


// ---------------------------------------------------------------- destructor

tzMesh::~tzMesh(void) {}


