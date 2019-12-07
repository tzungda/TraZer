#version 410

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadowMap_directional;
uniform samplerCube shadowMap_point;

uniform int lightSourceType;
uniform float near_plane;
uniform float far_plane;
uniform vec2 screenSize;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{           
	if (lightSourceType == 0) // directional light
	{
		float depthValue = texture(shadowMap_directional, TexCoords).r;
		FragColor = vec4(vec3(depthValue), 1.0); // orthographic
	}
	else if (lightSourceType == 1) // point light equirectangular
	{
#if 0
		// Equirectangular
		vec2 t = (TexCoords - 0.5f) * 2.0f;
		vec2 a = t * vec2(3.14159265, 1.57079633); // Convert to (lat, lon) angle
		vec2 c = cos(a), s = sin(a); // Convert to cartesian coordinates
		FragColor = texture(shadowMap_point, vec3(c.x * c.y, s.y, c.y * s.x));
#else 
		// Cubemap 6-side 
		if (TexCoords.x < 0.333 &&
			TexCoords.y >= 0.5)
		{
			vec2 t = vec2(TexCoords.x * 3, (TexCoords.y - 0.5) * 2);
			t = (t - 0.5) * 2;
			FragColor = texture(shadowMap_point, vec3(1, t.y, t.x)); // +X
		}
		else if (TexCoords.x < 0.333 &&
			TexCoords.y < 0.5)
		{
			vec2 t = vec2(TexCoords.x * 3, TexCoords.y * 2);
			t = (t - 0.5) * 2;
			FragColor = texture(shadowMap_point, vec3(-1, t.y, -t.x)); // -X
		}
		else if (TexCoords.x >= 0.333 && TexCoords.x < 0.666 &&
			TexCoords.y > 0.5)
		{
			vec2 t = vec2((TexCoords.x - 0.333) * 3, (TexCoords.y - 0.5) * 2);
			t = (t - 0.5) * 2;
			FragColor = texture(shadowMap_point, vec3(-t.x, 1, -t.y)); // +Y
		}
		else if (TexCoords.x >= 0.333 && TexCoords.x < 0.666 &&
			TexCoords.y < 0.5)
		{
			vec2 t = vec2((TexCoords.x - 0.333) * 3, TexCoords.y * 2);
			t = (t - 0.5) * 2;
			FragColor = texture(shadowMap_point, vec3(-t.x, -1, t.y)); // -Y
		}
		else if (TexCoords.x >= 0.666 &&
			TexCoords.y > 0.5)
		{
			vec2 t = vec2((TexCoords.x - 0.666) * 3, (TexCoords.y - 0.5) * 2);
			t = (t - 0.5) * 2;
			FragColor = texture(shadowMap_point, vec3(-t.x, t.y, 1)); // +Z
		}
		else if (TexCoords.x >= 0.666 &&
			TexCoords.y < 0.5)
		{
			vec2 t = vec2((TexCoords.x - 0.666) * 3, TexCoords.y * 2);
			t = (t - 0.5) * 2;
			FragColor = texture(shadowMap_point, vec3(t.x, t.y, -1)); // -Z
		}
#endif
		FragColor = vec4(vec3(FragColor.r), 1);
	}
}