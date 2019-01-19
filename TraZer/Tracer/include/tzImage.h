#ifndef TZ_TRACER_IMAGE
#define TZ_TRACER_IMAGE


#include <vector>		
#include "tzColor.h"
using namespace std;

//--------------------------------------------------------------------- class Image

class tzImage {	
	public:
	
		tzImage(void);

		tzImage(const tzImage& image);

		tzImage& operator= (const tzImage& image);

		~tzImage(void) ;
		
		void read_ppm_file(const char* file_name);

		void readPng( const char* fileName );
		
		int getHeight(void);	
		
		int getWidth(void);	
				
		tzColor getColor(const int row, const int col) const;		
		
	private:
		int 				mWidth;			// horizontal resolution of image
		int					mHeight;			// vertical resolution of image
		vector<tzColor> 	mPixels;
};


//===================================================================================
inline int tzImage::getHeight(void)
{
	return (mHeight);
}


//===================================================================================
inline int tzImage::getWidth(void) 
{
	return (mWidth);
}

#endif
		
