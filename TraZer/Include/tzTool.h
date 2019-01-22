#ifndef TZ_TOOL
#define TZ_TOOL

#include "lodepng/lodepng.h"
//#include "GLM/glm/glm.hpp"
#include "../Utilities/tzColor.h"

class tzTool
{
public:
	static bool writePng( const std::vector<tzColor>& buffer, unsigned int w, unsigned int h, std::string &outputPath );
};

#endif