#ifndef TZ_IMAGE_TOOL
#define TZ_IMAGE_TOOL

#include "lodepng/lodepng.h"
#include "tzCoreTexture.h"
#include "../Utilities/tzColor.h"
#include "../Utilities/tzMatrix.h"
#include "../Utilities/tzVector3D.h"

class tzTool
{
public:
	static bool writePng( const std::vector<tzColor>& buffer, unsigned int w, unsigned int h, std::string &outputPath );
	static tzTextureData loadPngTexture(const char* path);

	static tzMatrix lookAt( const tzVector3D& eye, const tzVector3D& center, const tzVector3D& up);
	static tzMatrix ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	static tzMatrix perspective( float fovy, float aspect, float zNear, float zFar);
};

#endif