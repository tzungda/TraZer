#ifndef TZ_OBJECT_H
#define TZ_OBJECT_H

#include <vector>
#include <string>

#include <rpc.h>

using namespace std;

/*
The basic class for all the object which will be rendered, such as mesh, light, 
*/

class tzObject
{
public:
	tzObject() ;
	virtual ~tzObject() {};

private:
	// members
	//unsigned int		mId;
	string				mId;
	string				mName;
	tzObject*			mParent;
	vector<tzObject*>	mChildren;
	

public:
	// object's name
	virtual string					name( ) const;
	virtual void					setName( const string &name );

	// parent
	virtual tzObject*				parent() const;
	virtual void					setParent(tzObject* parent);

	// children
	virtual unsigned int			numChildren() const;
	virtual tzObject*				childByIndex( unsigned int index ) const;
	virtual void					addChild( tzObject* child );
};

#endif