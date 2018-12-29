#ifndef TZ_MATERIAL
#define TZ_MATERIAL

#include <string>
#include "GLM/glm/glm.hpp"

using namespace std;
using namespace glm;


struct tzMaterial
{
	string name;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;
	float shininess;
	float ior;      // index of refraction
	float dissolve; // 1 == opaque; 0 == fully transparent
					  // illumination model (see http://www.fileformat.info/format/material/)
	int illum;

	int dummy; // Suppress padding warning.

	unsigned int m_tex_ambient;
	unsigned int m_tex_diffuse;
	unsigned int m_tex_specular;
	unsigned int m_tex_specular_highlight;
	unsigned int m_tex_bump;
	unsigned int m_tex_displacement;
	unsigned int m_tex_alpha;

	unsigned char hasAmbientTex;
	unsigned char hasDiffuseTex;
	unsigned char hasSpecularTex;

} ;

#endif
