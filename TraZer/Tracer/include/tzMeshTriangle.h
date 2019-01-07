#ifndef TZ_TRACER_MESH_TRIANGLE
#define TZ_TRACER_MESH_TRIANGLE


#include "../include/tzIGeometricObject.h"
#include "tzMesh.h"

class tzMeshTriangle: public tzIGeometricObject {	
	public:

		tzMesh* 	mesh_ptr;					// stores all the data
		int			index0, index1, index2;  	// indices into the vertices array in the mesh
		tzNormal	normal;					
		float		area;						// required for translucency
	
	public:
		
		tzMeshTriangle(void);
		
		tzMeshTriangle(tzMesh* _mesh_ptr, const int i1, const int i2, const int i3);

		virtual tzMeshTriangle*
		clone(void) const = 0;
	
		tzMeshTriangle(const tzMeshTriangle& mt);

		virtual
		~tzMeshTriangle(void);

		tzMeshTriangle&
		operator= (const tzMeshTriangle& rhs);
		
		virtual bool 												 
		hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const = 0;
		
		virtual	bool shadowHit(const tzRay& ray, float& tmin) const;
		
		void 
		compute_normal(const bool reverse_normal);
				
		virtual tzNormal
		get_normal(void) const;				

		virtual tzBBox
		get_bounding_box(void);
		
	protected:
	
		float  
		interpolate_u(const float beta, const float gamma) const;
		
		float  
		interpolate_v(const float beta, const float gamma) const;			
};

#endif

