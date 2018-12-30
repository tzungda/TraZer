// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the definition of the Grid class

#include <iostream>
#include <vector>
#include <math.h>

//#include "../include/tzMaths.h"
#include "../include/tzConstants.h"

#include "tzVector3D.h"
#include "tzPoint3D.h"
#include "tzGrid.h"

#include "../include/tzMeshTriangle.h"
#include "../include/tzFlatMeshTriangle.h"
#include "../include/tzSmoothMeshTriangle.h"
//#include "FlatUVMeshTriangle.h"
//#include "SmoothUVMeshTriangle.h"

#include "../include/tzTriangle.h"
#include "../include/tzSmoothTriangle.h"

#include "ply.h"

using namespace std;

typedef enum {
	flat, 
	smooth
} TriangleType;


double
_clamp(const double x, const double min, const double max) {
	return (x < min ? min : (x > max ? max : x));
}

// ----------------------------------------------------------------  default constructor

tzGrid::tzGrid(void)
	: 	tzCompound(),
		nx(0),
		ny(0),
		nz(0),
		mesh_ptr(new tzMesh),
		reverse_normal(false),
		mScale( 1.0f )
{
	// The cells array will be empty
}


// ----------------------------------------------------------------  constructor
// for rendering triangle meshes

tzGrid::tzGrid(tzMesh* _mesh_ptr)
	: 	tzCompound(),
		nx(0),
		ny(0),
		nz(0),
		mesh_ptr(_mesh_ptr),
		reverse_normal(false),
		mScale( 1.0f )
{
	// The cells array will be empty
}

// ---------------------------------------------------------------- clone

tzGrid*
tzGrid::clone(void) const {
	return (new tzGrid(*this));
}


// ---------------------------------------------------------------- copy constructor
// not implemented

tzGrid::tzGrid(const tzGrid& grid) {}


// ---------------------------------------------------------------- assignment operator
// not implemented

tzGrid&
tzGrid::operator= (const tzGrid& rhs)	{
	return (*this);
}	



// ---------------------------------------------------------------- destructor
// not implemented

tzGrid::~tzGrid(void) {}


tzBBox 
tzGrid::get_bounding_box(void) {
	return (bbox);
}

//------------------------------------------------------------------ setup_cells

void
tzGrid::setup_cells(void) {
	// find the minimum and maximum coordinates of the grid
	
	tzPoint3D p0 = find_min_bounds();
	tzPoint3D p1 = find_max_bounds();
	
	bbox.x0 = p0.x;
	bbox.y0 = p0.y;
	bbox.z0 = p0.z;
	bbox.x1 = p1.x;
	bbox.y1 = p1.y;
	bbox.z1 = p1.z;
		
	// compute the number of grid cells in the x, y, and z directions
	
	int num_objects = objects.size();
	
	// dimensions of the grid in the x, y, and z directions
	
	double wx = p1.x - p0.x;
	double wy = p1.y - p0.y;
	double wz = p1.z - p0.z;  
	
	double multiplier = 2.0;  	// multiplyer scales the number of grid cells relative to the number of objects
								
	double s = pow(wx * wy * wz / num_objects, 0.3333333);    
	nx = multiplier * wx / s + 1;
	ny = multiplier * wy / s + 1;
	nz = multiplier * wz / s + 1;

	// set up the array of grid cells with null pointers
	
	int num_cells = nx * ny * nz;	
	cells.reserve(num_objects);
	
	for (int j = 0; j < num_cells; j++)
		cells.push_back(NULL);
				
	// set up a temporary array to hold the number of objects stored in each cell
	
	vector<int> counts;
	counts.reserve(num_cells);
		
	for (int j = 0; j < num_cells; j++)
		counts.push_back(0);
		

	// put the objects into the cells
	 
	tzBBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index
			
	for (int j = 0; j < num_objects; j++) {
		obj_bBox =  objects[j]->get_bounding_box();
				
		// compute the cell indices at the corners of the bounding box of the object
		
		int ixmin = _clamp((obj_bBox.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymin = _clamp((obj_bBox.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmin = _clamp((obj_bBox.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
		int ixmax = _clamp((obj_bBox.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymax = _clamp((obj_bBox.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmax = _clamp((obj_bBox.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
				
		// add the object to the cells
				
		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {			// cells in x direction
					index = ix + nx * iy + nx * ny * iz;
															
					if (counts[index] == 0) {
						cells[index] = objects[j];
						counts[index] += 1;  						// now = 1
					}
					else {
						if (counts[index] == 1) {
							tzCompound* compound_ptr = new tzCompound;	// construct a compound object
							compound_ptr->add_object(cells[index]); // add object already in cell
							compound_ptr->add_object(objects[j]);  	// add the new object
							cells[index] = compound_ptr;			// store compound in current cell
							counts[index] += 1;  					// now = 2
						}						
						else {										// counts[index] > 1
							cells[index]->add_object(objects[j]);	// just add current object
							counts[index] += 1;						// for statistics only
						}
					}
				}	
	}  // end of for (int j = 0; j < num_objects; j++)
	
	
	// erase the Compound::vector that stores the object pointers, but don't delete the objects
	
	objects.erase (objects.begin(), objects.end());

	
// display some statistics on counts
// this is useful for finding out how many cells have no objects, one object, etc
// comment this out if you don't want to use it

	int num_zeroes 	= 0;
	int num_ones 	= 0;
	int num_twos 	= 0;
	int num_threes 	= 0;
	int num_greater = 0;
	
	for (int j = 0; j < num_cells; j++) {
		if (counts[j] == 0)
			num_zeroes += 1;
		if (counts[j] == 1)
			num_ones += 1;
		if (counts[j] == 2)
			num_twos += 1;
		if (counts[j] == 3)
			num_threes += 1;
		if (counts[j] > 3)
			num_greater += 1;
	}
	
	cout << "num_cells =" << num_cells << endl;
	cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << endl;  
	cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << endl;		

	// erase the temporary counts vector
	
	counts.erase (counts.begin(), counts.end());  
}


//------------------------------------------------------------------ find_min_bounds

// find the minimum grid coordinates, based on the bounding boxes of all the objects

tzPoint3D 
tzGrid::find_min_bounds(void) {
	tzBBox 	object_box;
	tzPoint3D p0(kHugeValue);

	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		object_box = objects[j]->get_bounding_box();
				
		if (object_box.x0 < p0.x)
			p0.x = object_box.x0;
		if (object_box.y0 < p0.y)
			p0.y = object_box.y0;
		if (object_box.z0 < p0.z)
			p0.z = object_box.z0;
	}
	
	p0.x -= kEpsilon; p0.y -= kEpsilon; p0.z -= kEpsilon;
		
	return (p0);
}


//------------------------------------------------------------------ find_max_bounds

// find the maximum grid coordinates, based on the bounding boxes of the objects

tzPoint3D 
tzGrid::find_max_bounds(void) {
	tzBBox object_box;
	tzPoint3D p1(-kHugeValue);

	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		object_box = objects[j]->get_bounding_box();
				
		if (object_box.x1 > p1.x)
			p1.x = object_box.x1;
		if (object_box.y1 > p1.y)
			p1.y = object_box.y1;
		if (object_box.z1 > p1.z)
			p1.z = object_box.z1;
	}
	
	p1.x += kEpsilon; p1.y += kEpsilon; p1.z += kEpsilon;	
		
	return (p1);
}


// The following functions read a file in PLY format, and construct mesh triangles where the data is stored 
// in the mesh object
// They are just small wrapper functions that call the functions read_ply_file or read_uv_ply_file that
// do the actual reading
// These use the PLY code by Greg Turk to read the PLY file


// ----------------------------------------------------------------------------- read_flat_triangles

void												
tzGrid::read_flat_triangles(char* file_name) {
  	read_ply_file(file_name, flat);
 }


// ----------------------------------------------------------------------------- read_smooth_triangles

void												
tzGrid::read_smooth_triangles(char* file_name) {
  	read_ply_file(file_name, smooth);
  	compute_mesh_normals();
}


// ----------------------------------------------------------------------------- read_ply_file

// Most of this function was written by Greg Turk and is released under the licence agreement 
// at the start of the PLY.h and PLY.c files
// The PLY.h file is #included at the start of this file
// It still has some of his printf statements for debugging
// I've made changes to construct mesh triangles and store them in the grid
// mesh_ptr is a data member of Grid
// objects is a data member of Compound
// triangle_type is either flat or smooth
// Using the one function construct to flat and smooth triangles saves a lot of repeated code
// The ply file is the same for flat and smooth triangles


void
tzGrid::read_ply_file(char* file_name, const int triangle_type) {
	// Vertex definition 
	
	typedef struct Vertex {
	  float x,y,z;      // space coordinates       
	} Vertex;
	
	// Face definition. This is the same for all files but is placed here to keep all the definitions together

	typedef struct Face {
	  	unsigned char nverts;    // number of vertex indices in list
	  	int* verts;              // vertex index list 
	} Face;
		
	// list of property information for a vertex
	// this varies depending on what you are reading from the file

	PlyProperty vert_props[] = {
	  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
	  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
	  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0}
	};

	// list of property information for a face. 
	// there is a single property, which is a list
	// this is the same for all files

	PlyProperty face_props[] = { 
	  	{"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
	   		1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)}
	};
	
	// local variables
	
	int 			i,j;
  	PlyFile*		ply;
  	int 			nelems;		// number of element types: 2 in our case - vertices and faces
  	char**			elist;
	int 			file_type;
	float 			version;
	int 			nprops;		// number of properties each element has
	int 			num_elems;	// number of each type of element: number of vertices or number of faces
	PlyProperty**	plist;
	Vertex**		vlist;
	Face**			flist;
	char*			elem_name;
	int				num_comments;
	char**			comments;
	int 			num_obj_info;
	char**			obj_info;


  	// open a ply file for reading
  
	ply = ply_open_for_reading(file_name, &nelems, &elist, &file_type, &version);
	
  	// print what we found out about the file
  
  	printf ("version %f\n", version);
  	printf ("type %d\n", file_type);
  	
  	// go through each kind of element that we learned is in the file and read them 

  	for (i = 0; i < nelems; i++) {  // there are only two elements in our files: vertices and faces
	    // get the description of the first element 
	    
  	    elem_name = elist[i];
	    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

	    // print the name of the element, for debugging
	    
		cout << "element name  " << elem_name << "  num elements = " << num_elems << "  num properties =  " << nprops << endl;

	    // if we're on vertex elements, read in the properties
    
    	if (equal_strings ("vertex", elem_name)) {
	      	// set up for getting vertex elements
	      	// the three properties are the vertex coordinates
	
			ply_get_property (ply, elem_name, &vert_props[0]);
	      	ply_get_property (ply, elem_name, &vert_props[1]);
		  	ply_get_property (ply, elem_name, &vert_props[2]);
		  	
		  	// reserve mesh elements
		  	
		  	mesh_ptr->num_vertices = num_elems;
		  	mesh_ptr->vertices.reserve(num_elems);

		  	// grab all the vertex elements
		  			  
		  	for (j = 0; j < num_elems; j++) {
				Vertex* vertex_ptr = new Vertex;

		        // grab an element from the file
		        
				ply_get_element (ply, (void *) vertex_ptr);
		  		mesh_ptr->vertices.push_back(tzPoint3D(vertex_ptr->x * mScale, vertex_ptr->y* mScale, vertex_ptr->z* mScale));
		  		delete vertex_ptr;
		  	}
    	}

	    // if we're on face elements, read them in 
	    
	    if (equal_strings ("face", elem_name)) {
		    // set up for getting face elements
		
			ply_get_property (ply, elem_name, &face_props[0]);   // only one property - a list
			
		  	mesh_ptr->num_triangles = num_elems;
		  	objects.reserve(num_elems);  // triangles will be stored in Compound::objects
		
			// the following code stores the face numbers that are shared by each vertex
		  	
		  	mesh_ptr->vertex_faces.reserve(mesh_ptr->num_vertices);
		  	vector<int> faceList;
		  			  	
		  	for (j = 0; j < mesh_ptr->num_vertices; j++) 
		  		mesh_ptr->vertex_faces.push_back(faceList); // store empty lists so that we can use the [] notation below
		  			
			// grab all the face elements
			
			int count = 0; // the number of faces read
					      
			for (j = 0; j < num_elems; j++) {
			    // grab an element from the file 
			    
			    Face* face_ptr = new Face;
			    
			    ply_get_element (ply, (void *) face_ptr);
			    
			    // construct a mesh triangle of the specified type
			    
			    if (triangle_type == flat) {
			    	tzFlatMeshTriangle* triangle_ptr = new tzFlatMeshTriangle(mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
					triangle_ptr->compute_normal(reverse_normal);		
					objects.push_back(triangle_ptr); 
				} 
			    	
			    if (triangle_type == smooth) {
			    	tzSmoothMeshTriangle* triangle_ptr = new tzSmoothMeshTriangle(mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
					triangle_ptr->compute_normal(reverse_normal); 	// the "flat triangle" normal is used to compute the average normal at each mesh vertex
					objects.push_back(triangle_ptr); 				// it's quicker to do it once here, than have to do it on average 6 times in compute_mesh_normals
					
					// the following code stores a list of all faces that share a vertex
					// it's used for computing the average normal at each vertex in order(num_vertices) time
					
					mesh_ptr->vertex_faces[face_ptr->verts[0]].push_back(count);
					mesh_ptr->vertex_faces[face_ptr->verts[1]].push_back(count);
					mesh_ptr->vertex_faces[face_ptr->verts[2]].push_back(count);
					count++;
				} 
			}
			
			if (triangle_type == flat)
				mesh_ptr->vertex_faces.erase(mesh_ptr->vertex_faces.begin(), mesh_ptr->vertex_faces.end());
	    }
    
	    // print out the properties we got, for debugging
	    
	    for (j = 0; j < nprops; j++)
	    	printf ("property %s\n", plist[j]->name);
	
	}  // end of for (i = 0; i < nelems; i++) 


	// grab and print out the comments in the file
	  
	comments = ply_get_comments (ply, &num_comments);
	  
	for (i = 0; i < num_comments; i++)
	    printf ("comment = '%s'\n", comments[i]);

	// grab and print out the object information
	  
	obj_info = ply_get_obj_info (ply, &num_obj_info);
	  
	for (i = 0; i < num_obj_info; i++)
	    printf ("obj_info = '%s'\n", obj_info[i]);

	// close the ply file 
	  
	ply_close (ply);
}



// ----------------------------------------------------------------------------- compute_mesh_normals
// this computes the average normal at each vertex
// the calculation is of order(num_vertices)
// some triangles in ply files are not defined properly

void
tzGrid::compute_mesh_normals(void) {
	mesh_ptr->normals.reserve(mesh_ptr->num_vertices);
	
	for (int index = 0; index < mesh_ptr->num_vertices; index++) {   // for each vertex
		tzNormal normal;    // is zero at this point	
			
		for (int j = 0; j < mesh_ptr->vertex_faces[index].size(); j++)
			normal += objects[mesh_ptr->vertex_faces[index][j]]->get_normal();  
	
		// The following code attempts to avoid (nan, nan, nan) normalised normals when all components = 0
		
		if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0)
			normal.y = 1.0;
		else 
			normal.normalize();     
		
		mesh_ptr->normals.push_back(normal);
	}
	
	// erase the vertex_faces arrays because we have now finished with them
	
	for (int index = 0; index < mesh_ptr->num_vertices; index++)
		for (int j = 0; j < mesh_ptr->vertex_faces[index].size(); j++)
			mesh_ptr->vertex_faces[index].erase (mesh_ptr->vertex_faces[index].begin(), mesh_ptr->vertex_faces[index].end());
	
	mesh_ptr->vertex_faces.erase (mesh_ptr->vertex_faces.begin(), mesh_ptr->vertex_faces.end());

	cout << "finished constructing normals" << endl;
}



// ------------------------------------------------------------------------------------------------  tesselate_flat_sphere
// tesselate a unit sphere into flat triangles that are stored directly in the grid

void												
tzGrid::tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps) {
	double pi = 3.1415926535897932384;
		
	// define the top triangles which all touch the north pole
	
	int k = 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	0, 1, 0);																		// top (north pole)
		
		tzPoint3D v1(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// bottom left
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
					
		tzPoint3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// bottom  right
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		tzTriangle* triangle_ptr = new tzTriangle(v0, v1, v2);
		objects.push_back(triangle_ptr); 							
	}
	
	
	// define the bottom triangles which all touch the south pole
	
	k = vertical_steps - 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// top left
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
						
		tzPoint3D v1(	0, -1, 0);																		// bottom (south pole)		
												
		tzPoint3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// top right 
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		tzTriangle* triangle_ptr = new tzTriangle(v0, v1, v2);
		objects.push_back(triangle_ptr);  						
	}

	
	
	//  define the other triangles
	
	for (int k = 1; k <= vertical_steps - 2; k++) {	
		for (int j = 0; j <= horizontal_steps - 1; j++) {
			// define the first triangle
		
			// vertices
			
			tzPoint3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 				// bottom left, use k + 1, j
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v1(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 		// bottom  right, use k + 1, j + 1
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v2(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 					// top left, 	use k, j
						cos(pi * k / vertical_steps), 
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
		
			tzTriangle* triangle_ptr1 = new tzTriangle(v0, v1, v2);
			objects.push_back(triangle_ptr1); 	
			
			
			// define the second triangle
		
			// vertices
			
			v0 = tzPoint3D(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 			// top right, use k, j + 1
							cos(pi * k / vertical_steps), 
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );
							
			v1 = tzPoint3D (	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 				// top left, 	use k, j
							cos(pi * k / vertical_steps), 
							cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
							
			v2 = tzPoint3D (	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 	// bottom  right, use k + 1, j + 1
							cos(pi * (k + 1) / vertical_steps), 
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
		
			tzTriangle* triangle_ptr2 = new tzTriangle(v0, v1, v2); 
			objects.push_back(triangle_ptr2); 						
		}
	}	
}


// ------------------------------------------------------------------------------------------------  tesselate_smooth_sphere
// tesselate a unit sphere into smooth triangles that are stored directly in the grid

void												
tzGrid::tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps) {
	double pi = 3.1415926535897932384;
	
	// define the top triangles
	
	int k = 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	0, 1, 0);																		// top
		
		tzPoint3D v1(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// bottom left
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
					
		tzPoint3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// bottom  right
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		tzSmoothTriangle* triangle_ptr = new tzSmoothTriangle(v0, v1, v2);
		triangle_ptr->n0 = v0;
		triangle_ptr->n1 = v1;
		triangle_ptr->n2 = v2;
		objects.push_back(triangle_ptr); 							
	}
	
	
	// define the bottom triangles
	
	k = vertical_steps - 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// top left
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
						
		tzPoint3D v1(	0, -1, 0);																		// bottom			
												
		tzPoint3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// top right 
					cos(pi * k / vertical_steps), 
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		tzSmoothTriangle* triangle_ptr = new tzSmoothTriangle(v0, v1, v2);
		triangle_ptr->n0 = v0;
		triangle_ptr->n1 = v1;
		triangle_ptr->n2 = v2;
		objects.push_back(triangle_ptr);  						
	}

	
	//  define the other triangles
	
	for (int k = 1; k <= vertical_steps - 2; k++) {	
		for (int j = 0; j <= horizontal_steps - 1; j++) {
			// define the first triangle
		
			// vertices
			
			tzPoint3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 				// bottom left, use k + 1, j
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v1(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 		// bottom  right, use k + 1, j + 1
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v2(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 					// top left, 	use k, j
						cos(pi * k / vertical_steps), 
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
		
			tzSmoothTriangle* triangle_ptr1 = new tzSmoothTriangle(v0, v1, v2);
			triangle_ptr1->n0 = v0;
			triangle_ptr1->n1 = v1;
			triangle_ptr1->n2 = v2;
			objects.push_back(triangle_ptr1); 	
			
			
			// define the second triangle
		
			// vertices
			
			v0 = tzPoint3D(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 			// top right, use k, j + 1
							cos(pi * k / vertical_steps), 
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );
							
			v1 = tzPoint3D (	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 				// top left, 	use k, j
							cos(pi * k / vertical_steps), 
							cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
							
			v2 = tzPoint3D (	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 	// bottom  right, use k + 1, j + 1
							cos(pi * (k + 1) / vertical_steps), 
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
		
			tzSmoothTriangle* triangle_ptr2 = new tzSmoothTriangle(v0, v1, v2); 
			triangle_ptr2->n0 = v0;
			triangle_ptr2->n1 = v1;
			triangle_ptr2->n2 = v2;
			objects.push_back(triangle_ptr2); 					
		}
	}	
}


// ---------------------------------------------------------------- hit

// The following grid traversal code is based on the pseudo-code in Shirley (2000)	
// The first part is the same as the code in BBox::hit

bool 							 
tzGrid::hit(const tzRay& ray, double& t, tzShadeRec& sr) const {
	double ox = ray.o.x;
	double oy = ray.o.y;
	double oz = ray.o.z;
	double dx = ray.d.x;
	double dy = ray.d.y;
	double dz = ray.d.z;

	double x0 = bbox.x0;
	double y0 = bbox.y0;
	double z0 = bbox.z0;
	double x1 = bbox.x1;
	double y1 = bbox.y1;
	double z1 = bbox.z1;
	
	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max; 
	
	// the following code includes modifications from Shirley and Morley (2003)
	
	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	double t0, t1;
	
	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;
		
	if (tz_min > t0)
		t0 = tz_min;
		
	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;
		
	if (tz_max < t1)
		t1 = tz_max;
			
	if (t0 > t1)
		return(false);
	
			
	// initial cell coordinates
	
	int ix, iy, iz;
	
	if (bbox.inside(ray.o)) {  			// does the ray start inside the grid?
		ix = _clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = _clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = _clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		tzPoint3D p = ray.o + t0 * ray.d;  // initial hit point with grid's bounding box
		ix = _clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = _clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = _clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	
	// ray parameter increments per cell in the x, y, and z directions
	
	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;
		
	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;
	
	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	
	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}
	
	
	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	
	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}
		
	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	
	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}
	
		
	// traverse the grid
	
	while (true) {	
		tzIGeometricObject* object_ptr = cells[ix + nx * iy + nx * ny * iz];
		
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->hit(ray, t, sr) && t < tx_next) {
				material_ptr = object_ptr->get_material();
				return (true);
			}
			
			tx_next += dtx;
			ix += ix_step;
						
			if (ix == ix_stop)
				return (false);
		} 
		else { 	
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->hit(ray, t, sr) && t < ty_next) {
					material_ptr = object_ptr->get_material();
					return (true);
				}
				
				ty_next += dty;
				iy += iy_step;
								
				if (iy == iy_stop)
					return (false);
		 	}
		 	else {		
				if (object_ptr && object_ptr->hit(ray, t, sr) && t < tz_next) {
					material_ptr = object_ptr->get_material();
					return (true);
				}
				
				tz_next += dtz;
				iz += iz_step;
								
				if (iz == iz_stop)
					return (false);
		 	}
		}
	}
}	// end of hit

	//===================================================================================
bool tzGrid::shadow_hit(const tzRay &ray, float &tmin) const
{
	double ox = ray.o.x;
	double oy = ray.o.y;
	double oz = ray.o.z;
	double dx = ray.d.x;
	double dy = ray.d.y;
	double dz = ray.d.z;

	double x0 = bbox.x0;
	double y0 = bbox.y0;
	double z0 = bbox.z0;
	double x1 = bbox.x1;
	double y1 = bbox.y1;
	double z1 = bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);


	// initial cell coordinates

	int ix, iy, iz;

	if (bbox.inside(ray.o)) {  			// does the ray start inside the grid?
		ix = _clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = _clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = _clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		tzPoint3D p = ray.o + t0 * ray.d;  // initial hit point with grid's bounding box
		ix = _clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = _clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = _clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}


	// traverse the grid

	while (true) {
		tzIGeometricObject* object_ptr = cells[ix + nx * iy + nx * ny * iz];

		if (object_ptr )
		{
			if (object_ptr->get_material( ) )
			{
				int a = 0;
				a = 1;
			}
		}

		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < tx_next) {
				//material_ptr = object_ptr->get_material();
				return (true);
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < ty_next) {
					//material_ptr = object_ptr->get_material();
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
				if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < tz_next) {
					//material_ptr = object_ptr->get_material();
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}

	return false;
}