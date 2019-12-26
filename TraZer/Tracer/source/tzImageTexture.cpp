
#include "../include/tzImageTexture.h"

// ---------------------------------------------------------------- default constructor

tzImageTexture::tzImageTexture(void)
	:	tzITexture(),
		mHeight(100),
		mWidth(100),
		mImagePtr(NULL),
		mMappingPtr(NULL)
{}


// ---------------------------------------------------------------- constructor

tzImageTexture::tzImageTexture(tzImage* imagePtr)
	:	tzITexture(),
	mHeight(imagePtr->getHeight()),
	mWidth(imagePtr->getWidth()),
	mImagePtr(imagePtr),
	mMappingPtr(NULL)
{}


// ---------------------------------------------------------------- copy constructor

tzImageTexture::tzImageTexture(const tzImageTexture& it)
	: 	tzITexture(it),
	mHeight(it.mHeight),
	mWidth(it.mWidth)
{
	if (it.mImagePtr)
		*mImagePtr = *it.mImagePtr;
	else
		mImagePtr = NULL;
		
	if (it.mMappingPtr)
		mMappingPtr = it.mMappingPtr->clone();
	else
		mMappingPtr = NULL;
}


// ---------------------------------------------------------------- assignment operator

tzImageTexture& tzImageTexture::operator= (const tzImageTexture& rhs)
{
	if (this == &rhs)
		return (*this);
	
	tzITexture::operator= (rhs);
	
	mHeight = rhs.mHeight;
	mWidth = rhs.mWidth;
	
	if (mImagePtr)
	{
		delete mImagePtr;
		mImagePtr = NULL;
	}
	
	if (rhs.mImagePtr)
	{
		*mImagePtr = *rhs.mImagePtr;
	}
	
	if (mMappingPtr) 
	{
		delete mMappingPtr;
		mMappingPtr = NULL;
	}
	
	if (rhs.mMappingPtr)
	{
		mMappingPtr = rhs.mMappingPtr->clone();
	}

	return (*this);
}


// ---------------------------------------------------------------- clone

tzImageTexture* tzImageTexture::clone(void) const 
{
	return (new tzImageTexture(*this));
}	


// ---------------------------------------------------------------- destructor

tzImageTexture::~tzImageTexture(void) 
{

	if (mImagePtr) {
		delete mImagePtr;
		mImagePtr = NULL;
	}
	
	if (mMappingPtr) {
		delete mMappingPtr;
		mMappingPtr = NULL;
	}
}


// ---------------------------------------------------------------- getColor

// When we ray trace a triangle mesh object with uv mapping, the mapping pointer may be NULL
// because we can define uv coordinates on an arbitrary triangle mesh.
// In this case we don't use the local hit point because the pixel coordinates are computed 
// from the uv coordinates stored in the ShadeRec object in the uv triangles' hit functions
// See, for example, Listing 29.12.

tzColor tzImageTexture::getColor(const tzShadeRec& sr) const 
{
	int row;
	int column;
		
	if (mMappingPtr)
	{
		mMappingPtr->getTexelCoordinates(sr.mLocalHitPoint, mHeight, mWidth, row, column);
	}
	else 
	{
		row 	= (int)(sr.mV * (mHeight - 1));
		column 	= (int)(sr.mU * (mWidth - 1));
	}
	
	row %= mHeight;
	if ( row < 0 )
		row += mHeight;
	column %= mWidth;
	if ( column < 0 )
		column += mWidth;

	return (mImagePtr->getColor(row, column));
}  





