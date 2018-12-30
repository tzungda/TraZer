#include "../Include/tzCoreObject.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreObject::tzCoreObject( )
{
	// create unique id
	UUID uuid;
	::UuidCreate(&uuid);
	// set it
	RPC_CSTR *str_id = 0;
	UuidToStringA( &uuid, str_id);
	mId = (char*)str_id;

	//
	mParent = NULL;
}


/*
Interfaces
*/

//===================================================================================
string tzCoreObject::name() const
{
	return mName;
}

//===================================================================================
void tzCoreObject::setName(const string &name)
{
	mName = name;
}

//===================================================================================
tzCoreObject* tzCoreObject::parent() const
{
	return mParent;
}

//===================================================================================
void tzCoreObject::setParent(tzCoreObject* parent)
{
	mParent = parent;
}

//===================================================================================
unsigned int tzCoreObject::numChildren() const
{
	return (unsigned int)mChildren.size();
}

//===================================================================================
tzCoreObject* tzCoreObject::childByIndex( unsigned int index ) const
{
	if ( index >= numChildren())
	{
		printf( "tzObject::childByIndex - the index is larger than numChildren()\n" );
		return NULL;
	}
	return mChildren[index];
}

//===================================================================================
void tzCoreObject::addChild(tzCoreObject* child )
{
	mChildren.push_back( child );
}