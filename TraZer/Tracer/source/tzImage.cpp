
#include <iostream>
#include <stdio.h>

#include "../include/tzConstants.h"   // defines red
#include "../include/tzImage.h"

// ---------------------------------------------------- default constructor

tzImage::tzImage(void)
	:	hres(100),
		vres(100)
{}


// ---------------------------------------------------- copy constructor								

tzImage::tzImage(const tzImage& image)
	:	hres(image.hres),
		vres(image.vres),
		pixels(image.pixels)
{}		


// ---------------------------------------------------- assignment operator	

tzImage&
tzImage::operator= (const tzImage& rhs) {
	if (this == &rhs)
		return (*this);
	
	hres 		= rhs.hres;
	vres 		= rhs.vres;
	pixels 		= rhs.pixels;

	return (*this);
}		

// ---------------------------------------------------- destructor	

tzImage::~tzImage(void)	{}
	

// ---------------------------------------------------- read_ppm_file

void										
tzImage::read_ppm_file(const char* file_name) {

    // read-only binary sequential access
    
    FILE* file = NULL;
	fopen_s(&file, file_name, "rb");
    
    if (file == 0){
		cout << "could not open file" << endl;
	}
	else
		cout << "file opened" << endl;

    // PPM header
    
    unsigned char ppm_type;
    if (fscanf_s(file, "P%c\n", &ppm_type, sizeof(ppm_type) ) != 1){
		cout << "Invalid PPM signature" << endl;
	}
	
    // only binary PPM supported
    
    if (ppm_type != '6'){
		cout << "Only binary PPM supported" << endl;
	}

    // skip comments
    
    unsigned char dummy;
    while (fscanf_s(file ,"#%c", &dummy, sizeof(dummy)))
        while (fgetc(file) != '\n');

    // read image size
    
    if (fscanf_s(file, "%d %d\n", &hres, &vres) != 2){
		cout << "Invalid image size" << endl;
	}

    if (hres <= 0)
		cout << "Invalid image width" << endl;
	else
		cout << "hres = " << hres << endl;

    
	if (vres <= 0)
		cout << "Invalid image height" << endl;
	else
		cout << "vres = " << vres << endl;


    // maximum value to be found in the PPM file (usually 255)
    
    unsigned int max_value;
    if (fscanf_s(file, "%d\n", &max_value) != 1){
		cout << "Invalid max value" << endl;
	}

	float inv_max_value = 1.0 / (float)max_value;

    // allocate memory
    
	pixels.reserve(hres * vres);

    // read pixel data
    
    for (unsigned int y = 0; y < vres; y++) {
        for (unsigned int x = 0; x < hres; x++) {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
            
			//
			int xx = fscanf_s(file, "%c", &red, sizeof(red) );
			xx = fscanf_s(file, "%c", &green, sizeof(red));
			xx = fscanf_s(file, "%c", &blue, sizeof(blue));
			

			/*
            if (fscanf_s(file, "%c%c%c", &red, &green, &blue, sizeof(int)*3) != 3) {
				cout << "Invalid image" << endl;
			}
			*/
			

			float r = red   * inv_max_value;
			float g = green * inv_max_value;
			float b = blue  * inv_max_value;

			pixels.push_back(tzRGBColor(r, g, b));
        }
    }

    // close file
    
    fclose(file);
	
	cout << "finished reading PPM file" << endl;
}



// --------------------------------------------------------------------------------------------- get_color 

tzRGBColor									
tzImage::get_color(const int row, const int column) const {
	int index = column + hres * (vres - row - 1);
	int pixels_size = pixels.size();
	
	if (index < pixels_size)
		return (pixels[index]);
	else
		return (red);    // useful for debugging 
}

 
