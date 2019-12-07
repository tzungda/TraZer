#version 410

in vec2 TexCoords;

uniform sampler2D diffuseTexture;

void main(void)
{
	float alpha = texture(diffuseTexture, TexCoords).a;
	if(alpha < 0.4)
		discard;
}
