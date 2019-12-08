#include "../Include/tzImageTool.h"
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include <iostream>

//===================================================================================
bool tzImageTool::writePng(const std::vector<tzColor>& buffer, unsigned int w, unsigned int h, std::string &outputPath)
{
	std::vector<unsigned char> my_image;
	const int _h = h;
	const int _w = w;
	my_image.resize(_h * _w * 4);
	tzColor buf;
	//int bufferIndex = 0;
	for (int y = 0; y < _h; y++)
	{
		for (int x = 0; x < _w; x++)
		{
			unsigned newpos = 4 * y * _w + 4 * x;
			int buffIndex = x + (_h - y - 1)*_w;
			buf = buffer[buffIndex];
			//bufferIndex++;
			{
				my_image[newpos + 0] = (unsigned char)(buf.r*255.0f); //R
				my_image[newpos + 1] = (unsigned char)(buf.g*255.0f); //G
				my_image[newpos + 2] = (unsigned char)(buf.b*255.0f); //B
				my_image[newpos + 3] = (unsigned char)(buf.a*255.0f); //A
			}
		}
	}
	std::vector<unsigned char> png;
	unsigned _error = lodepng::encode(png, my_image, _w, _h);

	if (_error)
	{
		std::cout << "PNG encoding error " << _error << ": " << lodepng_error_text(_error) << std::endl;
		return false;
	}

	lodepng::save_file(png, outputPath);

	return true;
}

//===================================================================================
tzTextureData tzImageTool::LoadPngTexture(const char* path)
{
	tzTextureData texture;
	std::string spath = path;
	if (spath != "")
	{
		printf("Load Image %s", path);
		int n;
		stbi_uc *data = stbi_load(path, &texture.width, &texture.height, &n, 4);
		if (data != NULL)
		{
			printf(" ~ Success\n");
			texture.data = new unsigned char[texture.width * texture.height * 4 * sizeof(unsigned char)];
			memcpy(texture.data, data, texture.width * texture.height * 4 * sizeof(unsigned char));
			// vertical-mirror image data
			for (size_t i = 0; i < texture.width; i++)
			{
				for (size_t j = 0; j < texture.height / 2; j++)
				{
					for (size_t k = 0; k < 4; k++) {
						std::swap(texture.data[(j * texture.width + i) * 4 + k], texture.data[((texture.height - j - 1) * texture.width + i) * 4 + k]);
					}
				}
			}
			stbi_image_free(data);
		}
		else
			printf(" ~ Failed\n");
	}
	return texture;
}


