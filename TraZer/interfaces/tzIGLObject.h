#ifndef TZ_GL_OBJECT_INTERFACE
#define TZ_GL_OBJECT_INTERFACE

//#include "Common.h"
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include "../Include/tzCoreObject.h"

#include <vector>

class tzIGLObject
{
public:
	tzIGLObject( )
	{ 
		mPtrCoreObject = nullptr;
	}
	virtual ~tzIGLObject() {}
protected:
	std::shared_ptr<tzCoreObject> mPtrCoreObject;

public:
	virtual	void									setCoreObject( std::shared_ptr<tzCoreObject> coreObjectPtr) = 0;
	virtual	std::shared_ptr<tzCoreObject>			coreObject() const
	{
		return mPtrCoreObject;
	}
};

#endif