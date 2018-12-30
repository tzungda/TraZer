#ifndef TZ_TRACER_GRID
#define TZ_TRACER_GRID


#include "../include/tzCompound.h"
#include "../include/tzShadeRec.h"
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
		get_bounding_box(void);

		void												
		read_flat_triangles(char* file_name);

		void												
		read_smooth_triangles(char* file_name);

		void												
		tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps);

		void												
		tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps);

		virtual bool 
		hit(const tzRay& ray, double& tmin, tzShadeRec& sr) const;		 					 

		virtual bool shadow_hit(const tzRay &ray, float &tmin) const;

		void
		setup_cells(void);

		void
		reverse_mesh_normals(void);

		void
		store_material(tzIMaterial* material, const int index); 							

		void	setScale( float scale );

	private: 

		vector<tzIGeometricObject*>	cells;			// grid of cells
		int							nx, ny, nz;    	// number of cells in the x, y, and z directions
		tzBBox						bbox;			// bounding box
		tzMesh*						mesh_ptr;		// holds triangle data
		bool						reverse_normal;	// some PLY files have normals that point inwards
		float						mScale;

		tzPoint3D
		find_min_bounds(void);

		tzPoint3D
		find_max_bounds(void);

		void
		read_ply_file(char* file_name, const int triangle_type);

		void
		compute_mesh_normals(void);	


};


// ------------------------------------------------------------------------------ store_material

inline void
tzGrid::reverse_mesh_normals(void) {
	reverse_normal = true;
}


// ------------------------------------------------------------------------------ store_material
// stores the material in the specified object
// this is called from the Rosette and Archway classes, which inherit from Grid

inline void
tzGrid::store_material(tzIMaterial* material_ptr, const int index) {
	objects[index]->set_material(material_ptr); 
}

inline void
tzGrid::setScale(float scale)
{
	mScale = scale;
}

#endif









