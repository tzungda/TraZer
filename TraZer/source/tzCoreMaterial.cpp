
#include "../include/tzCoreMaterial.h"

/*
constructor/destructor
*/

//===================================================================================
tzCoreMaterial::tzCoreMaterial()
{
}

//===================================================================================
tzCoreMaterial::~tzCoreMaterial()
{
}


/*
interfaces
*/

//===================================================================================
void tzCoreMaterial::setAttribute( const string &name, float value )
{
	mAttributeList[name] = value;
}

//===================================================================================
const map<string, float>& tzCoreMaterial::attributes( ) const
{
	return mAttributeList;
}



