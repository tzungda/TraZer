#ifndef TZ_IMAGE_TOOL
#define TZ_IMAGE_TOOL

#include "lodepng/lodepng.h"
#include "tzCoreTexture.h"
#include "../Utilities/tzColor.h"


class tzImageTool
{
public:
	static bool writePng( const std::vector<tzColor>& buffer, unsigned int w, unsigned int h, std::string &outputPath );
	static tzTextureData LoadPngTexture(const char* path);
};

#endif