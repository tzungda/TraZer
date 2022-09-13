#ifndef TZ_TRACER_GRID
#define TZ_TRACER_GRID


#include "../include/tzCompound.h"
#include "../include/tzShadeRec.h"
#include "../include/tzCoreMesh.h"
#include "tzMesh.h"


class tzGrid: public tzCompound 
{										  	
public:
	tzGrid(void);

	tzGrid(std::shared_ptr<tzMesh> _mesh_ptr);

	virtual std::shared_ptr<tzIGeometricObject> clone(void) const;

	tzGrid(const tzGrid& rg);

	tzGrid&
	operator= (const tzGrid& rhs);

	virtual 										
	~tzGrid(void);

	virtual tzBBox getBoundingBox(void) const;


	void												
	tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps);

	void												
	tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps);

	virtual bool 
	hit(const tzRay& ray, float& tmin, tzShadeRec& sr) ;

	virtual bool shadowHit(const tzRay &ray, const tzShadeRec& sr, float &tmin) const;

	void
	setupCells(void);

	void reverse_mesh_normals(void);

	void store_material(std::shared_ptr < tzIMaterial > material, const int index, int threadId);

	void	setScale( float scale );

	// add mesh
	void addMesh(	const std::vector<tzPoint3D> &vertices,
					const std::vector<tzNormal> &normals,
					const std::vector<float> &u,
					const std::vector<float> &v,
					const std::vector<std::vector<int> > &vertex_faces,
					const std::vector< tzCoreMesh::index > &face_vertices,
					const int &num_vertices,
					const int &num_triangles,
					const tzMatrix &matrix,
					std::shared_ptr < tzIMaterial> material,
					std::shared_ptr<tzITexture> alphaTexture = nullptr);

private: 

	std::vector<std::shared_ptr<tzIGeometricObject>>	mCells;			// grid of cells
	int							mNx, mNy, mNz;    	// number of cells in the x, y, and z directions
	tzBBox						mBbox;			// bounding box
	std::shared_ptr<tzMesh>						mMeshPtr;		// holds triangle data
	bool						mReverseNormal;	
	float						mScale;

	std::vector<std::shared_ptr<tzMesh>>		mMeshList;

	tzPoint3D findMinBounds(void);

	tzPoint3D findMaxBounds(void);

	void computeMeshNormals(void);	

};

//===================================================================================
inline void tzGrid::reverse_mesh_normals(void) 
{
	mReverseNormal = true;
}

//===================================================================================
inline void tzGrid::store_material(std::shared_ptr < tzIMaterial> materialPtr, const int index, int threadId)
{
	mObjects[index]->setMaterial(materialPtr, threadId);
}

//===================================================================================
inline void tzGrid::setScale(float scale)
{
	mScale = scale;
}

#endif

