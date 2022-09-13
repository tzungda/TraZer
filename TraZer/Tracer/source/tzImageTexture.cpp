
#include "../include/tzImageTexture.h"

//===================================================================================
tzImageTexture::tzImageTexture(void)
	:	tzITexture(),
		mHeight(100),
		mWidth(100),
		mImagePtr(nullptr),
		mMappingPtr(nullptr)
{}

//===================================================================================
tzImageTexture::tzImageTexture(tzImage* imagePtr)
	:	tzITexture(),
	mHeight(imagePtr->getHeight()),
	mWidth(imagePtr->getWidth()),
	mImagePtr(imagePtr),
	mMappingPtr(nullptr)
{}

//===================================================================================
tzImageTexture::tzImageTexture(const tzImageTexture& it)
	: 	tzITexture(it),
	mHeight(it.mHeight),
	mWidth(it.mWidth)
{
	if (it.mImagePtr)
		*mImagePtr = *it.mImagePtr;
	else
		mImagePtr = nullptr;
		
	if (it.mMappingPtr)
		mMappingPtr = it.mMappingPtr->clone();
	else
		mMappingPtr = nullptr;
}

//===================================================================================
tzImageTexture& tzImageTexture::operator= (const tzImageTexture& rhs)
{
	if (this == &rhs)
		return (*this);
	
	tzITexture::operator= (rhs);
	
	mHeight = rhs.mHeight;
	mWidth = rhs.mWidth;

	
	if (rhs.mImagePtr)
	{
		*mImagePtr = *rhs.mImagePtr;
	}

	
	if (rhs.mMappingPtr)
	{
		mMappingPtr = rhs.mMappingPtr->clone();
	}

	return (*this);
}

//===================================================================================
std::shared_ptr<tzITexture> tzImageTexture::clone(void) const 
{
	return (std::make_shared< tzImageTexture >(*this));
}

//===================================================================================
tzImageTexture::~tzImageTexture(void) 
{
}

//===================================================================================
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

