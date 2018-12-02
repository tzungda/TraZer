#include "../Include/tzObject.h"

/*
Constructor
*/

//===================================================================================
tzObject::tzObject( )
{
	mParent = NULL;
}


/*
Interfaces
*/

//===================================================================================
tzObject* tzObject::parent() const
{
	return mParent;
}

//===================================================================================
void tzObject::setParent(tzObject* parent)
{
	mParent = parent;
}

//===================================================================================
unsigned int tzObject::numChildren() const
{
	return (unsigned int)mChildren.size();
}

//===================================================================================
tzObject* tzObject::childByIndex( unsigned int index ) const
{
	if ( index >= numChildren())
	{
		printf( "tzObject::childByIndex - the index is larger than numChildren()\n" );
		return NULL;
	}
	return mChildren[index];
}

//===================================================================================
void tzObject::addChild( tzObject* child )
{
	mChildren.push_back( child );
}