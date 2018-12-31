#ifndef TZ_TRACER_IMAGE
#define TZ_TRACER_IMAGE


#include <vector>		
#include "tzRGBColor.h"
using namespace std;

//--------------------------------------------------------------------- class Image

class tzImage {	
	public:
	
		tzImage(void);

		tzImage(const tzImage& image);

		tzImage&
		operator= (const tzImage& image);

		~tzImage(void) ;
		
		void										
		read_ppm_file(const char* file_name);
		
		int
		get_hres(void);	
		
		int
		get_vres(void);	
				
		tzRGBColor									
		get_color(const int row, const int col) const;		
		
	private:
		int 				hres;			// horizontal resolution of image
		int					vres;			// vertical resolution of image
		vector<tzRGBColor> 	pixels;
};


//--------------------------------------------------------------------- get_hres

inline int
tzImage::get_hres(void) {
	return (hres);
}


//--------------------------------------------------------------------- get_vres

inline int
tzImage::get_vres(void) {
	return (vres);
}

#endif
		
