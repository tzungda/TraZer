#include "../Include/tzObject.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzObject::tzObject( )
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
string tzObject::name() const
{
	return mName;
}

//===================================================================================
void tzObject::setName(const string &name)
{
	mName = name;
}

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