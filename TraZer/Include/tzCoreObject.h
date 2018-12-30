#ifndef TZ_CORE_OBJECT
#define TZ_CORE_OBJECT

#include <vector>
#include <string>

#include <rpc.h>

using namespace std;

/*
The basic class for all the object which will be rendered, such as mesh, light, 
*/

class tzCoreObject
{
public:
	tzCoreObject() ;
	virtual ~tzCoreObject() {};

private:
	// members
	//unsigned int		mId;
	string				mId;
	string				mName;
	tzCoreObject*			mParent;
	vector<tzCoreObject*>	mChildren;
	

public:
	// object's name
	virtual string					name( ) const;
	virtual void					setName( const string &name );

	// parent
	virtual tzCoreObject*			parent() const;
	virtual void					setParent(tzCoreObject* parent);

	// children
	virtual unsigned int			numChildren() const;
	virtual tzCoreObject*			childByIndex( unsigned int index ) const;
	virtual void					addChild(tzCoreObject* child );
};

#endif