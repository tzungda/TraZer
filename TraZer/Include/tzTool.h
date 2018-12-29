#ifndef TZ_TOOL
#define TZ_TOOL

#include "lodepng/lodepng.h"
#include "GLM/glm/glm.hpp"

class tzTool
{
public:
	static bool writePng( const std::vector<glm::vec4>& buffer, unsigned int w, unsigned int h, std::string &outputPath );
};

#endif