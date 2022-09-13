#version 410

in vec4 FragPos;
in vec2 FragTexCoords;

layout(location = 0) out vec4 fragColor;

uniform sampler2D diffuseTexture;

uniform float farPlane;
uniform vec3 lightPos;

void main()
{
	float alpha = texture(diffuseTexture, FragTexCoords).a;
	if (alpha < 0.4)
	{
		discard;
	}
	else
	{
		float lightDistance = length(FragPos.xyz - lightPos);

		// map to [0;1] range by dividing by far_plane
		lightDistance = lightDistance / farPlane;

		// write this as modified depth
		gl_FragDepth = lightDistance;
	}
}