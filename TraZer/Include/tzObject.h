#ifndef TZ_OBJECT_H
#define TZ_OBJECT_H

#include <vector>
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
	tzObject*		  mParent;
	vector<tzObject*> mChildren;

public:
	// parent
	tzObject*				parent() const;
	void					setParent(tzObject* parent);

	// children
	unsigned int			numChildren() const;
	tzObject*				childByIndex( unsigned int index ) const;
	void					addChild( tzObject* child );
};

#endif