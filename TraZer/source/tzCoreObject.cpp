#include "../Include/tzCoreObject.h"

/*
Constructor/Destructor
*/

//===================================================================================
tzCoreObject::tzCoreObject( )
{
	// create unique id
	/*
	UUID uuid;
	::UuidCreate(&uuid);
	// set it
	RPC_CSTR *str_id = 0;
	UuidToStringA( &uuid, str_id);
	mId = (char*)str_id;
	*/

	//
	mParent = nullptr;
}


/*
Interfaces
*/

//===================================================================================
std::string tzCoreObject::name() const
{
	return mName;
}

//===================================================================================
void tzCoreObject::setName(const std::string &name)
{
	mName = name;
}

//===================================================================================
std::shared_ptr<tzCoreObject> tzCoreObject::parent() const
{
	return mParent;
}

//===================================================================================
void tzCoreObject::setParent( std::shared_ptr<tzCoreObject> parent)
{
	mParent = parent;
}

//===================================================================================
unsigned int tzCoreObject::numChildren() const
{
	return (unsigned int)mChildren.size();
}

//===================================================================================
std::shared_ptr<tzCoreObject> tzCoreObject::childByIndex( unsigned int index ) const
{
	if ( index >= numChildren())
	{
		printf( "tzObject::childByIndex - the index is larger than numChildren()\n" );
		return nullptr;
	}
	return mChildren[index];
}

//===================================================================================
void tzCoreObject::addChild( std::shared_ptr<tzCoreObject> child )
{
	mChildren.push_back( child );
}