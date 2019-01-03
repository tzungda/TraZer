#ifndef TZ_TRACER_COMPOUND
#define TZ_TRACER_COMPOUND

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include <vector> 

#include "../include/tzIGeometricObject.h"

//-------------------------------------------------------------------------------- class Compound

class tzCompound: public tzIGeometricObject {
	public:
		
		tzCompound(void);

		virtual tzCompound*
		clone(void) const;
	
		tzCompound(const tzCompound& c);

		~tzCompound(void);

		tzCompound&
		operator= (const tzCompound& c);
		
		virtual void 
		set_material(tzIMaterial* material_ptr);
		
		virtual void 											
		add_object(tzIGeometricObject* object_ptr);  
		
		int
		get_num_objects(void);						

		virtual bool 															 
		hit(const tzRay& ray, double& tmin, tzShadeRec& s) const;

		virtual	bool shadowHit(const tzRay& ray, float& tmin) const;
			
	protected:		
		
		std::vector<tzIGeometricObject*> objects;
		
	private:
	
		void 													
		delete_objects(void);								

		void												
		copy_objects(const std::vector<tzIGeometricObject*>& rhs_objects);
	
};


// ------------------------------------------------------------------------------- get_num_objects

inline int
tzCompound::get_num_objects(void) {
	return ((int)objects.size());
}

#endif
