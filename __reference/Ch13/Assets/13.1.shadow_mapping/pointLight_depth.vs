#version 410

layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec2 iv2tex_coord;

uniform mat4 model;

out vec2 TexCoords;

void main()
{
	gl_Position = model * vec4(iv3vertex, 1.0);
	TexCoords = iv2tex_coord;
}