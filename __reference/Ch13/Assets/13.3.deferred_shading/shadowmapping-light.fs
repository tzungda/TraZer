#version 410

in vec2 TexCoords;

//layout (location = 0) out vec4 color;

uniform sampler2D diffuseTexture;

void main(void)
{
	float alpha = texture(diffuseTexture, TexCoords).a;
	if(alpha < 0.4)
		discard;

    //color = vec4(gl_FragCoord.z);
}
