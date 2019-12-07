#version 410

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuseTexture;

void main()
{    
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gDiffuse.rgb = texture(diffuseTexture, TexCoords).rgb;
	gDiffuse.a = 1.0;
}
