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
		mPtrCoreObject = NULL;
	}
	virtual ~tzIGLObject() {}
protected:
	tzCoreObject		*mPtrCoreObject;

public:
	virtual	void					setCoreObject(tzCoreObject *coreObjectPtr) = 0;
	virtual	tzCoreObject*			coreObject() const
	{
		return mPtrCoreObject;
	}
};

#endif