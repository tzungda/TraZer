// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "../include/tzIBTDF.h"

// ------------------------------------------------------------------------ default constructor

tzIBTDF::tzIBTDF(void) {}


// ------------------------------------------------------------------------ copy constructor

tzIBTDF::tzIBTDF(const tzIBTDF& tzIBTDF) {}


// ------------------------------------------------------------------------ destructor

tzIBTDF::~tzIBTDF(void) {}


// ------------------------------------------------------------------------ assignment operator

tzIBTDF&														
tzIBTDF::operator= (const tzIBTDF& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ------------------------------------------------------------------------ f

tzColor
tzIBTDF::f(const tzShadeRec& sr, const tzVector3D& wo, const tzVector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------------ sampleF

tzColor
tzIBTDF::sampleF(const tzShadeRec& sr, const tzVector3D& wo, tzVector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------------ rho	
	
tzColor
tzIBTDF::rho(const tzShadeRec& sr, const tzVector3D& wo) const {
	return (black);
}
