// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the definition of the Grid class

#include <iostream>
#include <vector>
#include <math.h>

#include "../include/tzMaths.h"
#include "../include/tzConstants.h"

#include "tzVector3D.h"
#include "tzPoint3D.h"
#include "tzGrid.h"

#include "../include/tzMeshTriangle.h"
#include "../include/tzFlatMeshTriangle.h"
#include "../include/tzSmoothMeshTriangle.h"
#include "../include/tzFlatUVMeshTriangle.h"
#include "../include/tzSmoothUVMeshTriangle.h"

#include "../include/tzTriangle.h"
#include "../include/tzSmoothTriangle.h"


typedef enum {
	flat, 
	smooth
} TriangleType;

//===================================================================================
tzGrid::tzGrid(void)
	: 	tzCompound(),
		mNx(0),
		mNy(0),
		mNz(0),
		mMeshPtr( std::make_shared< tzMesh >()),
		mReverseNormal(false),
		mScale( 1.0f )
{
}

//===================================================================================
tzGrid::tzGrid(std::shared_ptr<tzMesh> meshPtr)
	: 	tzCompound(),
		mNx(0),
		mNy(0),
		mNz(0),
		mMeshPtr(meshPtr),
		mReverseNormal(false),
		mScale( 1.0f )
{
}

//===================================================================================
std::shared_ptr<tzIGeometricObject> tzGrid::clone(void) const 
{
	return (std::make_shared< tzGrid >(*this));
}

//===================================================================================
tzGrid::tzGrid(const tzGrid& grid) 
{
}

//===================================================================================
tzGrid& tzGrid::operator= (const tzGrid& rhs)	
{
	return (*this);
}	

//===================================================================================
tzGrid::~tzGrid(void) 
{
}

//===================================================================================
tzBBox tzGrid::getBoundingBox(void) const
{
	return (mBbox);
}

//===================================================================================
void tzGrid::setupCells(void) 
{
	tzPoint3D p0 = findMinBounds();
	tzPoint3D p1 = findMaxBounds();
	
	mBbox.x0 = p0.x;
	mBbox.y0 = p0.y;
	mBbox.z0 = p0.z;
	mBbox.x1 = p1.x;
	mBbox.y1 = p1.y;
	mBbox.z1 = p1.z;
		
	// compute the number of grid cells in the x, y, and z directions
	
	int numObjects = (int)mObjects.size();
	
	// dimensions of the grid in the x, y, and z directions
	
	float wx = p1.x - p0.x;
	float wy = p1.y - p0.y;
	float wz = p1.z - p0.z;
	
	float multiplier = 2.0f;  	// multiplyer scales the number of grid cells relative to the number of objects
								
	float s = pow(wx * wy * wz / numObjects, 0.3333333f);
	mNx = (int)(multiplier * wx / s) + 1;
	mNy = (int)(multiplier * wy / s) + 1;
	mNz = (int)(multiplier * wz / s) + 1;

	// set up the array of grid cells with null pointers
	
	int num_cells = mNx * mNy * mNz;	
	mCells.resize(num_cells);
	
	//for (int j = 0; j < num_cells; j++)
	//	mCells.push_back(nullptr);
				
	// set up a temporary array to hold the number of objects stored in each cell
	
	std::vector<int> counts;
	counts.resize(num_cells);
	memset( &counts[0], 0, sizeof( int )* num_cells );
		
	//for (int j = 0; j < num_cells; j++)
	//	counts.push_back(0);
		

	// put the objects into the cells
	 
	tzBBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index
			
	//for (int j = 0; j < numObjects; j++) 
	for (std::vector<std::shared_ptr<tzIGeometricObject> >::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it )
	{
		obj_bBox = (*it)->getBoundingBox();
				
		// compute the cell indices at the corners of the bounding box of the object
		
		int ixmin = (int)clamp((obj_bBox.x0 - p0.x) * (float)(mNx) / (p1.x - p0.x), 0.0f, (float)(mNx) - 1.0f);
		int iymin = (int)clamp((obj_bBox.y0 - p0.y) * (float)(mNy) / (p1.y - p0.y), 0.0f, (float)(mNy) - 1.0f);
		int izmin = (int)clamp((obj_bBox.z0 - p0.z) * (float)(mNz) / (p1.z - p0.z), 0.0f, (float)(mNz) - 1.0f);
		int ixmax = (int)clamp((obj_bBox.x1 - p0.x) * (float)(mNx) / (p1.x - p0.x), 0.0f, (float)(mNx) - 1.0f);
		int iymax = (int)clamp((obj_bBox.y1 - p0.y) * (float)(mNy) / (p1.y - p0.y), 0.0f, (float)(mNy) - 1.0f);
		int izmax = (int)clamp((obj_bBox.z1 - p0.z) * (float)(mNz) / (p1.z - p0.z), 0.0f, (float)(mNz) - 1.0f);
				
		// add the object to the cells
				
		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {			// cells in x direction
					index = ix + mNx * iy + mNx * mNy * iz;
															
					if (counts[index] == 0) {
						mCells[index] = (*it);
						counts[index] += 1;  						// now = 1
					}
					else {
						if (counts[index] == 1) {
							std::shared_ptr<tzCompound> compound_ptr = std::make_shared< tzCompound >();	// construct a compound object
							compound_ptr->addObject(mCells[index]); // add object already in cell
							compound_ptr->addObject((*it));  	// add the new object
							mCells[index] = compound_ptr;			// store compound in current cell
							counts[index] += 1;  					// now = 2
						}						
						else {										// counts[index] > 1
							mCells[index]->addObject((*it));	// just add current object
							counts[index] += 1;						// for statistics only
						}
					}
				}	
	}  // end of for (int j = 0; j < numObjects; j++)
	
	
	// erase the Compound::vector that stores the object pointers, but don't delete the objects
	
	mObjects.erase (mObjects.begin(), mObjects.end());

	
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
	
	std::cout << "num_cells =" << num_cells << std::endl;
	std::cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << std::endl;  
	std::cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << std::endl;		

	// erase the temporary counts vector
	
	counts.erase (counts.begin(), counts.end());  
}

//===================================================================================
tzPoint3D  tzGrid::findMinBounds(void) 
{
	tzBBox 	object_box;
	tzPoint3D p0(kHugeValue);

	int numObjects = (int)mObjects.size();
	
	for (int j = 0; j < numObjects; j++) 
	{
		object_box = mObjects[j]->getBoundingBox();
				
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


//===================================================================================
tzPoint3D tzGrid::findMaxBounds(void) 
{
	tzBBox object_box;
	tzPoint3D p1(-kHugeValue);

	int numObjects = (int)mObjects.size();
	
	for (int j = 0; j < numObjects; j++) {
		object_box = mObjects[j]->getBoundingBox();
				
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

//===================================================================================
void tzGrid::computeMeshNormals(void) 
{
	mMeshPtr->mNormals.reserve(mMeshPtr->mNumVertices);
	
	for (int index = 0; index < mMeshPtr->mNumVertices; index++) {   // for each vertex
		tzNormal normal;    // is zero at this point	
			
		for (int j = 0; j < mMeshPtr->mVertexFaces[index].size(); j++)
			normal += mObjects[mMeshPtr->mVertexFaces[index][j]]->getNormal();
	
		// The following code attempts to avoid (nan, nan, nan) normalised normals when all components = 0
		
		if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0)
			normal.y = 1.0;
		else 
			normal.normalize();     
		
		mMeshPtr->mNormals.push_back(normal);
	}
	
	// erase the vertex_faces arrays because we have now finished with them
	
	for (int index = 0; index < mMeshPtr->mNumVertices; index++)
		for (int j = 0; j < mMeshPtr->mVertexFaces[index].size(); j++)
			mMeshPtr->mVertexFaces[index].erase (mMeshPtr->mVertexFaces[index].begin(), mMeshPtr->mVertexFaces[index].end());
	
	mMeshPtr->mVertexFaces.erase (mMeshPtr->mVertexFaces.begin(), mMeshPtr->mVertexFaces.end());

	std::cout << "finished constructing normals" << std::endl;
}

//===================================================================================
void tzGrid::tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps) 
{
	float pi = 3.1415926535897932384f;
		
	// define the top triangles which all touch the north pole
	
	int k = 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0( 0, 1, 0);																		// top (north pole)
		
		tzPoint3D v1( sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// bottom left
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
					
		tzPoint3D v2( sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// bottom  right
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		std::shared_ptr<tzTriangle> triangle_ptr = std::make_shared< tzTriangle >(v0, v1, v2);
		mObjects.push_back(triangle_ptr);
	}
	
	
	// define the bottom triangles which all touch the south pole
	
	k = vertical_steps - 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// top left
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
						
		tzPoint3D v1(	0.0f, -1.0f, 0.0f);																		// bottom (south pole)		
												
		tzPoint3D v2(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// top right 
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		std::shared_ptr<tzTriangle> triangle_ptr = std::make_shared< tzTriangle >(v0, v1, v2);
		mObjects.push_back(triangle_ptr);
	}

	
	
	//  define the other triangles
	
	for (int k = 1; k <= vertical_steps - 2; k++) {	
		for (int j = 0; j <= horizontal_steps - 1; j++) {
			// define the first triangle
		
			// vertices
			
			tzPoint3D v0(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 				// bottom left, use k + 1, j
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0f * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v1(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 		// bottom  right, use k + 1, j + 1
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v2(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 					// top left, 	use k, j
						cos(pi * k / vertical_steps), 
						cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
		
			std::shared_ptr<tzTriangle> triangle_ptr1 = std::make_shared< tzTriangle >(v0, v1, v2);
			mObjects.push_back(triangle_ptr1);
			
			
			// define the second triangle
		
			// vertices
			
			v0 = tzPoint3D(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 			// top right, use k, j + 1
							cos(pi * k / vertical_steps), 
							cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );
							
			v1 = tzPoint3D (	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 				// top left, 	use k, j
							cos(pi * k / vertical_steps), 
							cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
							
			v2 = tzPoint3D (	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 	// bottom  right, use k + 1, j + 1
							cos(pi * (k + 1) / vertical_steps), 
							cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
		
			std::shared_ptr<tzTriangle> triangle_ptr2 = std::make_shared< tzTriangle >(v0, v1, v2); 
			mObjects.push_back(triangle_ptr2); 						
		}
	}	
}

//===================================================================================
void tzGrid::tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps) 
{
	float pi = 3.1415926535897932384f;
	
	// define the top triangles
	
	int k = 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	0, 1, 0);																		// top
		
		tzPoint3D v1(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// bottom left
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
					
		tzPoint3D v2(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// bottom  right
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		std::shared_ptr<tzSmoothTriangle> triangle_ptr = std::make_shared< tzSmoothTriangle >(v0, v1, v2);
		triangle_ptr->n0 = v0;
		triangle_ptr->n1 = v1;
		triangle_ptr->n2 = v2;
		mObjects.push_back(triangle_ptr); 							
	}
	
	
	// define the bottom triangles
	
	k = vertical_steps - 1;
	
	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices
		
		tzPoint3D v0(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// top left
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
						
		tzPoint3D v1(	0, -1, 0);																		// bottom			
												
		tzPoint3D v2(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// top right 
					cos(pi * k / vertical_steps), 
					cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);
	
		std::shared_ptr<tzSmoothTriangle> triangle_ptr = std::make_shared< tzSmoothTriangle >(v0, v1, v2);
		triangle_ptr->n0 = v0;
		triangle_ptr->n1 = v1;
		triangle_ptr->n2 = v2;
		mObjects.push_back(triangle_ptr);  						
	}

	
	//  define the other triangles
	
	for (int k = 1; k <= vertical_steps - 2; k++) {	
		for (int j = 0; j <= horizontal_steps - 1; j++) {
			// define the first triangle
		
			// vertices
			
			tzPoint3D v0(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 				// bottom left, use k + 1, j
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0f * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v1(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 		// bottom  right, use k + 1, j + 1
						cos(pi * (k + 1) / vertical_steps), 
						cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
				
			tzPoint3D v2(	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 					// top left, 	use k, j
						cos(pi * k / vertical_steps), 
						cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
		
			std::shared_ptr<tzSmoothTriangle> triangle_ptr1 = std::make_shared< tzSmoothTriangle >(v0, v1, v2);
			triangle_ptr1->n0 = v0;
			triangle_ptr1->n1 = v1;
			triangle_ptr1->n2 = v2;
			mObjects.push_back(triangle_ptr1); 	
			
			
			// define the second triangle
		
			// vertices
			
			v0 = tzPoint3D(	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 			// top right, use k, j + 1
							cos(pi * k / vertical_steps), 
							cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );
							
			v1 = tzPoint3D (	sin(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 				// top left, 	use k, j
							cos(pi * k / vertical_steps), 
							cos(2.0f * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);
							
			v2 = tzPoint3D (	sin(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 	// bottom  right, use k + 1, j + 1
							cos(pi * (k + 1) / vertical_steps), 
							cos(2.0f * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);
		
			std::shared_ptr<tzSmoothTriangle> triangle_ptr2 = std::make_shared< tzSmoothTriangle>(v0, v1, v2); 
			triangle_ptr2->n0 = v0;
			triangle_ptr2->n1 = v1;
			triangle_ptr2->n2 = v2;
			mObjects.push_back(triangle_ptr2); 					
		}
	}	
}

//===================================================================================
bool tzGrid::hit(const tzRay& ray, float& t, tzShadeRec& sr)  
{
	float ox = ray.mOrigin.x;
	float oy = ray.mOrigin.y;
	float oz = ray.mOrigin.z;
	float dx = ray.mDirection.x;
	float dy = ray.mDirection.y;
	float dz = ray.mDirection.z;

	float x0 = mBbox.x0;
	float y0 = mBbox.y0;
	float z0 = mBbox.z0;
	float x1 = mBbox.x1;
	float y1 = mBbox.y1;
	float z1 = mBbox.z1;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;
	
	// the following code includes modifications from Shirley and Morley (2003)
	
	float a = 1.0f / dx;
	if (a >= 0.0f) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	float t0, t1;
	
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
	
	if (mBbox.inside(ray.mOrigin)) // does the ray start inside the grid?
	{  			
		ix = (int)clamp((ox - x0) * (float)(mNx) / (x1 - x0), 0.0f, (float)(mNx - 1));
		iy = (int)clamp((oy - y0) * (float)(mNy) / (y1 - y0), 0.0f, (float)(mNy - 1));
		iz = (int)clamp((oz - z0) * (float)(mNz) / (z1 - z0), 0.0f, (float)(mNz - 1));
	}
	else {
		tzPoint3D p = ray.mOrigin + t0 * ray.mDirection;  // initial hit point with grid's bounding box
		ix = (int)clamp((p.x - x0) * (float)(mNx) / (x1 - x0), 0.0f, (float)(mNx - 1));
		iy = (int)clamp((p.y - y0) * (float)(mNy) / (y1 - y0), 0.0f, (float)(mNy - 1));
		iz = (int)clamp((p.z - z0) * (float)(mNz) / (z1 - z0), 0.0f, (float)(mNz - 1));
	}
	

	const float invNx = 1.0f / (float)mNx;
	const float invNy = 1.0f / (float)mNy;
	const float invNz = 1.0f / (float)mNz;

	// ray parameter increments per cell in the x, y, and z directions
	
	float dtx = (tx_max - tx_min) * invNx;
	float dty = (ty_max - ty_min) * invNy;
	float dtz = (tz_max - tz_min) * invNz;
		
	float 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;
	
	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = mNx;
	}
	else {
		tx_next = tx_min + (mNx - ix) * dtx;
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
		iy_stop = mNy;
	}
	else {
		ty_next = ty_min + (mNy - iy) * dty;
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
		iz_stop = mNz;
	}
	else {
		tz_next = tz_min + (mNz - iz) * dtz;
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
		std::shared_ptr<tzIGeometricObject> object_ptr = mCells[ix + mNx * iy + mNx * mNy * iz];
		
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr )
			{
				if ( object_ptr->hit(ray, t, sr) && t < tx_next) {
					mMaterialPtr[ray.mThreadId] = object_ptr->getMaterial(ray.mThreadId);
					return (true);
				}
			}
			
			tx_next += dtx;
			ix += ix_step;
						
			if (ix == ix_stop)
				return (false);
		} 
		else { 	
			if (ty_next < tz_next) {
				if ( object_ptr )
				{
					if ( object_ptr->hit(ray, t, sr) && t < ty_next) {
						mMaterialPtr[ray.mThreadId] = object_ptr->getMaterial(ray.mThreadId);
						return (true);
					}
				}
				
				ty_next += dty;
				iy += iy_step;
								
				if (iy == iy_stop)
					return (false);
		 	}
		 	else {	
				if (object_ptr)
				{
					if ( object_ptr->hit(ray, t, sr) && t < tz_next) {
						mMaterialPtr[ray.mThreadId] = object_ptr->getMaterial(ray.mThreadId);
						return (true);
					}
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
bool tzGrid::shadowHit(const tzRay &ray, const tzShadeRec& sr, float &tmin) const
{
	float ox = ray.mOrigin.x;
	float oy = ray.mOrigin.y;
	float oz = ray.mOrigin.z;
	float dx = ray.mDirection.x;
	float dy = ray.mDirection.y;
	float dz = ray.mDirection.z;

	float x0 = mBbox.x0;
	float y0 = mBbox.y0;
	float z0 = mBbox.z0;
	float x1 = mBbox.x1;
	float y1 = mBbox.y1;
	float z1 = mBbox.z1;

	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;

	// the following code includes modifications from Shirley and Morley (2003)

	float a = 1.0f / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	float t0, t1;

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

	if (mBbox.inside(ray.mOrigin)) // does the ray start inside the grid?
	{  			
		ix = (int)clamp((ox - x0) * (float)(mNx) / (x1 - x0), 0, (float)(mNx - 1));
		iy = (int)clamp((oy - y0) * (float)(mNy) / (y1 - y0), 0, (float)(mNy - 1));
		iz = (int)clamp((oz - z0) * (float)(mNz) / (z1 - z0), 0, (float)(mNz - 1));
	}
	else 
	{
		tzPoint3D p = ray.mOrigin + t0 * ray.mDirection;  // initial hit point with grid's bounding box
		ix = (int)clamp((p.x - x0) * (float)(mNx) / (x1 - x0), 0, (float)(mNx - 1));
		iy = (int)clamp((p.y - y0) * (float)(mNy) / (y1 - y0), 0, (float)(mNy - 1));
		iz = (int)clamp((p.z - z0) * (float)(mNz) / (z1 - z0), 0, (float)(mNz - 1));
	}

	const float invNx = 1.0f / (float)mNx;
	const float invNy = 1.0f / (float)mNy;
	const float invNz = 1.0f / (float)mNz;

	// ray parameter increments per cell in the x, y, and z directions

	float dtx = (tx_max - tx_min) * invNx;
	float dty = (ty_max - ty_min) * invNy;
	float dtz = (tz_max - tz_min) * invNz;

	float 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = mNx;
	}
	else {
		tx_next = tx_min + (mNx - ix) * dtx;
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
		iy_stop = mNy;
	}
	else {
		ty_next = ty_min + (mNy - iy) * dty;
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
		iz_stop = mNz;
	}
	else {
		tz_next = tz_min + (mNz - iz) * dtz;
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
		std::shared_ptr<tzIGeometricObject> object_ptr = mCells[ix + mNx * iy + mNx * mNy * iz];

		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->shadowHit(ray, sr, tmin) && tmin < tx_next) {
				//mMaterialPtr = object_ptr->getMaterial();
				return (true);
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->shadowHit(ray, sr, tmin) && tmin < ty_next) {
					//mMaterialPtr = object_ptr->getMaterial();
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
				if (object_ptr && object_ptr->shadowHit(ray, sr, tmin) && tmin < tz_next) {
					//mMaterialPtr = object_ptr->getMaterial();
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


//===================================================================================
void tzGrid::addMesh(const std::vector<tzPoint3D> &vertices,
	const std::vector<tzNormal> &normals,
	const std::vector<float> &u,
	const std::vector<float> &v,
	const std::vector<std::vector<int> > &vertex_faces,
	const std::vector< tzCoreMesh::index > &face_vertices,
	const int &num_vertices,
	const int &num_triangles,
	const tzMatrix &matrix,
	std::shared_ptr < tzIMaterial> material,
	std::shared_ptr<tzITexture> alphaTexture )
{
	
	std::shared_ptr<tzMesh> meshPtr = std::make_shared< tzMesh >();
	this->mMeshList.push_back( meshPtr );

	// update transformation
	meshPtr->mVertices.resize(vertices.size());
	const tzPoint3D *origP = vertices.data();
	tzPoint3D *newP = &(meshPtr->mVertices[0]);
	for ( int i = 0; i < (int)vertices.size(); i++ )
	{
		//meshPtr->mVertices[i] = vertices[i]* matrix;
		(*newP++) = (*origP++)*matrix;
	}
	//
	tzMatrix rotMatrix = matrix.rotationMatrix();
	meshPtr->mNormals.resize(normals.size());
	const tzNormal *origN = normals.data();
	tzNormal *newN = &(meshPtr->mNormals[0]);
	for (int i = 0; i < (int)normals.size(); i++)
	{
		//meshPtr->mNormals[i] = normals[i]* rotMatrix;
		(*newN++) = (*origN++)*rotMatrix;
	}

	//
	meshPtr->mUs = u;
	meshPtr->mVs = v;
	meshPtr->mVertexFaces = vertex_faces;
	meshPtr->mNumVertices = num_vertices;
	meshPtr->mNumTriangles = num_triangles;

	// create triangles
	size_t origSize = mObjects.size();
	//mObjects.resize( origSize + num_triangles );
	const tzCoreMesh::index *ptrIdx = face_vertices.data();
	for ( int tr = 0; tr < num_triangles; tr++ )
	{
		//int index = tr*3;
		int v0 = (*ptrIdx).vertex_index;
		int n0 = (*ptrIdx).normal_index;
		int uv0 = (*ptrIdx).texcoord_index;
		ptrIdx++;
		//
		int v1 = (*ptrIdx).vertex_index;
		int n1 = (*ptrIdx).normal_index;
		int uv1 = (*ptrIdx).texcoord_index;
		ptrIdx++;
		//
		int v2 = (*ptrIdx).vertex_index;
		int n2 = (*ptrIdx).normal_index;
		int uv2 = (*ptrIdx).texcoord_index;
		ptrIdx++;
		
		//
		std::shared_ptr<tzFlatUVMeshTriangle> triangle_ptr = std::make_shared< tzFlatUVMeshTriangle>(meshPtr, v0, v1, v2, n0, n1, n2, uv0, uv1, uv2);
		triangle_ptr->setMaterial( material, ~0 );
		if (alphaTexture )
		{
			triangle_ptr->setAlphaTexture(alphaTexture );
		}
		
		triangle_ptr->computeNormal(mReverseNormal);
		//mObjects[origSize + tr] = triangle_ptr;
		mObjects.push_back( triangle_ptr );
	}
}
