#ifndef TZ_TRACER_GRID
#define TZ_TRACER_GRID


#include "../include/tzCompound.h"
#include "../include/tzShadeRec.h"
#include "../include/tzCoreMesh.h"
#include "tzMesh.h"


//---------------------------------------------------------------------- class Grid

class tzGrid: public tzCompound {										  	
	public:

		tzGrid(void);

		tzGrid(tzMesh* _mesh_ptr);

		virtual tzGrid*
		clone(void) const;

		tzGrid(const tzGrid& rg);

		tzGrid&
		operator= (const tzGrid& rhs);

		virtual 										
		~tzGrid(void);

		virtual tzBBox 
		getBoundingBox(void);

		void												
		read_flat_triangles(char* file_name);

		void												
		read_smooth_triangles(char* file_name);

		void												
		tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps);

		void												
		tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps);

		virtual bool 
		hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const;

		virtual bool shadowHit(const tzRay &ray, float &tmin) const;

		void
		setupCells(void);

		void
		reverse_mesh_normals(void);

		void
		store_material(tzIMaterial* material, const int index); 							

		void	setScale( float scale );

		// uv related
		//void read_flat_uv_triangles(char* file_name);
		//void read_smooth_uv_triangles(char* file_name);

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
						tzIMaterial* material,
						tzITexture* alphaTexture = NULL);

	private: 

		std::vector<tzIGeometricObject*>	mCells;			// grid of cells
		int							mNx, mNy, mNz;    	// number of cells in the x, y, and z directions
		tzBBox						mBbox;			// bounding box
		tzMesh*						mMeshPtr;		// holds triangle data
		bool						mReverseNormal;	// some PLY files have normals that point inwards
		float						mScale;

		std::vector<tzMesh*>		mMeshList;

		tzPoint3D findMinBounds(void);

		tzPoint3D findMaxBounds(void);

		//void read_ply_file(char* file_name, const int triangle_type);

		void computeMeshNormals(void);	

		// uv related
		//void read_uv_ply_file(char* file_name, const int triangle_type);
};


// ------------------------------------------------------------------------------ store_material

inline void
tzGrid::reverse_mesh_normals(void) 
{
	mReverseNormal = true;
}


// ------------------------------------------------------------------------------ store_material
// stores the material in the specified object
// this is called from the Rosette and Archway classes, which inherit from Grid

inline void
tzGrid::store_material(tzIMaterial* mMaterialPtr, const int index) {
	mObjects[index]->setMaterial(mMaterialPtr);
}

inline void tzGrid::setScale(float scale)
{
	mScale = scale;
}

#endif









