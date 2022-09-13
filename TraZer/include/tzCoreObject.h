#ifndef TZ_CORE_OBJECT
#define TZ_CORE_OBJECT

#include <vector>
#include <string>

#include <rpc.h>

#include <memory>

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
	std::string				mId;
	std::string				mName;
	std::shared_ptr<tzCoreObject>			mParent;
	std::vector<std::shared_ptr<tzCoreObject>>	mChildren;
	

public:
	// object's name
	virtual std::string					name( ) const;
	virtual void					setName( const std::string &name );

	// parent
	virtual std::shared_ptr<tzCoreObject>			parent() const;
	virtual void					setParent(std::shared_ptr<tzCoreObject> parent);

	// children
	virtual unsigned int			numChildren() const;
	virtual std::shared_ptr<tzCoreObject>			childByIndex( unsigned int index ) const;
	virtual void					addChild(std::shared_ptr<tzCoreObject> child );
};

#endif