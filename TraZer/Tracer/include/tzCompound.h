#ifndef TZ_TRACER_COMPOUND
#define TZ_TRACER_COMPOUND


#include <vector> 

#include "../include/tzIGeometricObject.h"


class tzCompound: public tzIGeometricObject {
	public:
		
		tzCompound(void);

		virtual tzCompound* clone(void) const;
	
		tzCompound(const tzCompound& c);

		~tzCompound(void);

		tzCompound& operator= (const tzCompound& c);
		
		virtual void  setMaterial(tzIMaterial* mMaterialPtr);
		
		virtual void  addObject(tzIGeometricObject* object_ptr);  
		
		int getNumObjects(void);						

		virtual bool hit(const tzRay& ray, float& tmin, tzShadeRec& s) const;

		virtual	bool shadowHit(const tzRay& ray, float& tmin) const;
			
	protected:		
		
		std::vector<tzIGeometricObject*> objects;
		
	private:
	
		void deleteObjects(void);								

		void copyObjects(const std::vector<tzIGeometricObject*>& rhs_objects);
	
};


// ------------------------------------------------------------------------------- getNumObjects

inline int tzCompound::getNumObjects(void) 
{
	return ((int)objects.size());
}

#endif
