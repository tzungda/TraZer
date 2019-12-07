#version 410

layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec2 iv2tex_coord;
layout(location = 2) in vec3 iv3normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform int lightSourceType; // 0 for directional light, 1 for point light

out VertexData
{
    vec3 N; // eye space normal
    vec3 L; // eye space light vector
    vec3 H; // eye space halfway vector
    vec2 texcoord;
	vec3 worldSpacePos;
	vec4 lightSpacePos;
} vertexData;

void main()
{
	vec4 worldPosition = model * vec4(iv3vertex, 1.0);
	gl_Position = projection * view * worldPosition;
    vertexData.texcoord = iv2tex_coord;
	vertexData.worldSpacePos = worldPosition.xyz;
	vertexData.lightSpacePos = lightSpaceMatrix * worldPosition;
    vertexData.N = iv3normal;
}