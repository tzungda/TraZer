#version 410

out vec4 FragColor;

in vec2 TexCoords;

uniform int gBuffer_type;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gDepth;

uniform float near_plane;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
	return (2.0 * near_plane) / (far_plane + near_plane - depth * (far_plane - near_plane));
}

void main()
{             
	if(gBuffer_type == 1)
	{
		// Position buffer
		FragColor = vec4(texture(gPosition, TexCoords).rgb, 1);
	}
	else if(gBuffer_type == 2)
	{
		// Normal buffer
		FragColor = vec4(texture(gNormal, TexCoords).rgb, 1);
	}
	else if(gBuffer_type == 3)
	{
		// Diffuse Color buffer
		FragColor = vec4(texture(gDiffuse, TexCoords).rgb, 1);
	}
	else if(gBuffer_type == 4)
	{
		// Depth buffer
		float linearized_depth = LinearizeDepth(texture(gDepth, TexCoords).r);
		FragColor = vec4(vec3(linearized_depth), 1);
	}

	//FragColor = vec4(1, 0, 0, 1);
}