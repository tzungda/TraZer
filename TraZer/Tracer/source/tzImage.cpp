
#include <iostream>
#include <stdio.h>

#include "../include/tzConstants.h"   // defines red
#include "../include/tzImage.h"
#include "lodepng/lodepng.h"

//===================================================================================
tzImage::tzImage(void)
	:	mHeight(100),
		mWidth(100)
{}


//===================================================================================
tzImage::tzImage(const tzImage& image)
	:	mHeight(image.mHeight),
		mWidth(image.mWidth),
		pixels(image.pixels)
{}		


//===================================================================================
tzImage& tzImage::operator= (const tzImage& rhs) 
{
	if (this == &rhs)
		return (*this);
	
	mHeight 		= rhs.mHeight;
	mWidth 		= rhs.mWidth;
	pixels 		= rhs.pixels;

	return (*this);
}		

//===================================================================================
tzImage::~tzImage(void)	
{}
	

//===================================================================================
void tzImage::read_ppm_file(const char* file_name)
{

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
    
    if (fscanf_s(file, "%d %d\n", &mHeight, &mWidth) != 2){
		cout << "Invalid image size" << endl;
	}

    if (mHeight <= 0)
		cout << "Invalid image width" << endl;
	else
		cout << "mHeight = " << mHeight << endl;

    
	if (mWidth <= 0)
		cout << "Invalid image height" << endl;
	else
		cout << "mWidth = " << mWidth << endl;


    // maximum value to be found in the PPM file (usually 255)
    
    unsigned int max_value;
    if (fscanf_s(file, "%d\n", &max_value) != 1){
		cout << "Invalid max value" << endl;
	}

	float inv_max_value = 1.0f / (float)max_value;

    // allocate memory
    
	pixels.reserve(mHeight * mWidth);

    // read pixel data
    
    for ( int y = 0; y < mWidth; y++) {
        for ( int x = 0; x < mHeight; x++) {
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

//===================================================================================
void tzImage::readPng(const char* fileName)
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	lodepng::State state; //optionally customize this one

	unsigned error = lodepng::load_file(png, fileName); //load the image file with given filename
	if (!error) error = lodepng::decode(image, width, height, state, png);

	// set pixels
	mWidth = (int)width;
	mHeight = (int)height;
	pixels.resize(width*height);
	unsigned int len = width*height;
	for ( int i = 0; i < len; i++ )
	{
		int index = i*4;
		float r = ((float)image[index]) / 255.0f;
		float g = ((float)image[index+1]) / 255.0f;
		float b = ((float)image[index+2]) / 255.0f;
		float a = ((float)image[index+3]) / 255.0f;

		//
		pixels[i] = tzRGBColor( r, g, b, a );
	}
}

//===================================================================================
tzRGBColor tzImage::getColor(const int row, const int column) const 
{
	int index = column + mHeight * (mWidth - row - 1);
	int pixels_size = pixels.size();
	
	if (index < pixels_size)
		return (pixels[index]);
	else
		return (red);    // useful for debugging 
}

 
