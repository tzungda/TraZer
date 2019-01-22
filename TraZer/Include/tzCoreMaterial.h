#ifndef TZ_CORE_MATERIAL
#define TZ_CORE_MATERIAL


#include <map>
#include <string>
#include "../include/tzCoreObject.h"

using namespace std;


class tzCoreMaterial: public tzCoreObject
{
private:
	map<string, float>		mAttributeList;

public:
	/*
	constructor/destructor
	*/
	tzCoreMaterial();
	virtual ~tzCoreMaterial();

public:
	/*
	interfaces
	*/
	void	setAttribute( const string &name, float value );

	const map<string, float>& attributes( ) const;

} ;

#endif
