#version 410

layout(location = 0) in vec3 iv3vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vec4 worldPosition = model * vec4(iv3vertex, 1.0);
	gl_Position = projection * view * worldPosition;
}