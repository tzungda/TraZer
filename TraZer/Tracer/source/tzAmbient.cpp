

#include "../include/tzAmbient.h"

//===================================================================================
tzAmbient::tzAmbient(void)
	: 	tzILight(),
		mLs(1.0),
		mColor(1.0)
{}


//===================================================================================
tzAmbient::tzAmbient(const tzAmbient& a)
	: 	tzILight(a),
		mLs(a.mLs),
		mColor(a.mColor)
{}


//===================================================================================
tzILight* tzAmbient::clone(void) const
{
	return (new tzAmbient(*this));
}	


//===================================================================================
tzAmbient& tzAmbient::operator= (const tzAmbient& rhs) 
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	mLs = rhs.mLs;
	mColor = rhs.mColor;
	
	return (*this);
}


//===================================================================================
tzAmbient::~tzAmbient(void) {}


//===================================================================================
tzVector3D tzAmbient::getDirection(  tzShadeRec& s)
{
	return (tzVector3D(0.0));
}


//===================================================================================
tzColor tzAmbient::L(  tzShadeRec& sr) 
{
	return (mLs * mColor);
}





