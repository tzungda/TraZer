#ifndef TZ_TRACER_IMAGE_TEXTURE
#define TZ_TRACER_IMAGE_TEXTURE

#include "../include/tzImage.h"
#include "../include/tzIMapping.h"
#include "../include/tzITexture.h"

class tzImageTexture: public tzITexture {
	public:
	
		tzImageTexture(void);
			
		tzImageTexture(tzImage* _mImagePtr);
				
		tzImageTexture(const tzImageTexture& it);

		tzImageTexture& operator= (const tzImageTexture& rhs);

		virtual std::shared_ptr<tzITexture> clone(void) const;				

		virtual ~tzImageTexture(void) ;
		
		virtual tzColor getColor(const tzShadeRec& sr) const;
						
		void set_image(std::shared_ptr<tzImage> _mImagePtr);
		
		void set_mapping(std::shared_ptr<tzIMapping> map_ptr);	
		
	private:
	
		int 		mHeight;			// horizontal resolution of the image
		int			mWidth;			// vertical resolution of the image
		std::shared_ptr<tzImage>	mImagePtr;		// the image
		std::shared_ptr<tzIMapping>	mMappingPtr;	// mapping technique used, if any
};

//===================================================================================
inline void tzImageTexture::set_image( std::shared_ptr<tzImage> imagePtr) 
{
	mImagePtr = imagePtr;
	mHeight = imagePtr->getHeight();
	mWidth = imagePtr->getWidth();
}

//===================================================================================
inline void tzImageTexture::set_mapping(std::shared_ptr<tzIMapping> map_ptr) 
{
	mMappingPtr = map_ptr;
}

#endif		