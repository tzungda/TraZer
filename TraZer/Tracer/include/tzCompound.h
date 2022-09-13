#ifndef TZ_TRACER_COMPOUND
#define TZ_TRACER_COMPOUND


#include <vector> 

#include "../include/tzIGeometricObject.h"


class tzCompound: public tzIGeometricObject {
	public:
		
		tzCompound(void);

		virtual std::shared_ptr<tzIGeometricObject>  clone(void) const;
	
		tzCompound(const tzCompound& c);

		~tzCompound(void);

		tzCompound& operator= (const tzCompound& c);
		
		virtual void  setMaterial(std::shared_ptr < tzIMaterial> mMaterialPtr, int threadId);
		
		virtual void  addObject(std::shared_ptr<tzIGeometricObject>  object_ptr);
		
		int getNumObjects(void);						

		virtual bool hit(const tzRay& ray, float& tmin, tzShadeRec& s) ;

		virtual	bool shadowHit(const tzRay& ray, const tzShadeRec& sr, float& tmin ) const;
			
	protected:		
		
		std::vector<std::shared_ptr<tzIGeometricObject> > mObjects;
		
	private:
	
		void deleteObjects(void);								

		void copyObjects(const std::vector<std::shared_ptr<tzIGeometricObject> >& rhsObjects);
	
};

//===================================================================================
inline int tzCompound::getNumObjects(void) 
{
	return ((int)mObjects.size());
}

#endif
