
#include "../include/tzImageTexture.h"

// ---------------------------------------------------------------- default constructor

tzImageTexture::tzImageTexture(void)
	:	tzITexture(),
		hres(100),
		vres(100),
		image_ptr(NULL),
		mapping_ptr(NULL)
{}


// ---------------------------------------------------------------- constructor

tzImageTexture::tzImageTexture(tzImage* _image_ptr)
	:	tzITexture(),
		hres(_image_ptr->getHeight()),
		vres(_image_ptr->getWidth()),
		image_ptr(_image_ptr),
		mapping_ptr(NULL)
{}


// ---------------------------------------------------------------- copy constructor

tzImageTexture::tzImageTexture(const tzImageTexture& it)
	: 	tzITexture(it),
		hres(it.hres),
		vres(it.vres)
{
	if (it.image_ptr)
		*image_ptr = *it.image_ptr;
	else
		image_ptr = NULL;
		
	if (it.mapping_ptr)
		mapping_ptr = it.mapping_ptr->clone();
	else
		mapping_ptr = NULL;
}


// ---------------------------------------------------------------- assignment operator

tzImageTexture&
tzImageTexture::operator= (const tzImageTexture& rhs) {
	if (this == &rhs)
		return (*this);
	
	tzITexture::operator= (rhs);
	
	hres = rhs.hres;
	vres = rhs.vres;
	
	if (image_ptr) {
		delete image_ptr;
		image_ptr = NULL;
	}
	
	if (rhs.image_ptr)
		*image_ptr = *rhs.image_ptr;
	
	if (mapping_ptr) {
		delete mapping_ptr;
		mapping_ptr = NULL;
	}
	
	if (rhs.mapping_ptr)
		mapping_ptr = rhs.mapping_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

tzImageTexture*
tzImageTexture::clone(void) const {
	return (new tzImageTexture(*this));
}	


// ---------------------------------------------------------------- destructor

tzImageTexture::~tzImageTexture(void) {

	if (image_ptr) {
		delete image_ptr;
		image_ptr = NULL;
	}
	
	if (mapping_ptr) {
		delete mapping_ptr;
		mapping_ptr = NULL;
	}
}


// ---------------------------------------------------------------- getColor

// When we ray trace a triangle mesh object with uv mapping, the mapping pointer may be NULL
// because we can define uv coordinates on an arbitrary triangle mesh.
// In this case we don't use the local hit point because the pixel coordinates are computed 
// from the uv coordinates stored in the ShadeRec object in the uv triangles' hit functions
// See, for example, Listing 29.12.

tzRGBColor														
tzImageTexture::getColor(const tzShadeRec& sr) const {
	int row;
	int column;
		
	if (mapping_ptr)
		mapping_ptr->get_texel_coordinates(sr.mLocalHitPoint, hres, vres, row, column);
	else {
		row 	= (int)(sr.v * (vres - 1));  	
		column 	= (int)(sr.u * (hres - 1));	
	}
	
	return (image_ptr->getColor(row, column));
}  





