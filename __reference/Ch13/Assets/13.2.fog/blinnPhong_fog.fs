#version 410

layout(location = 0) out vec4 fragColor;

in VertexData
{
    vec3 N; // eye space normal
    vec3 L; // eye space light vector
    vec3 H; // eye space halfway vector
    vec2 texcoord;
	vec4 viewSpacePos;
	vec3 worldSpacePos;
	vec4 lightSpacePos;
} vertexData;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap_directional;
uniform samplerCube shadowMap_point;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int pcfKernel;
uniform int lightSourceType; // 0 for directional light, 1 for point light
uniform float farPlane;

uniform int fog_Type;
uniform vec4 fogColor;
uniform float fogDensity;
uniform float fog_start;
uniform float fog_end;
uniform vec2  bi_be_factor = vec2(0.004, 0.001);
float fogFactor;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float shadowContribution_directionalLight(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap_directional, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vertexData.N);
    vec3 lightDir = normalize(lightPos - vertexData.worldSpacePos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    
    float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap_directional, 0);
	if(pcfKernel > 0)
	{
		// PCF
		for(int x = -pcfKernel; x <= pcfKernel; ++x)
		{
			for(int y = -pcfKernel; y <= pcfKernel; ++y)
			{
				float pcfDepth = texture(shadowMap_directional, projCoords.xy + vec2(x, y) * texelSize).r;
				shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
			}    
		}
		shadow /= (pcfKernel * 2 + 1) * (pcfKernel * 2 + 1);
	}
    else
	{
		shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	}
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

float shadowContribution_pointLight(vec3 fragPos)
{
	// get vector between fragment position and light position
	vec3 fragToLight = fragPos - lightPos;
	float currentDepth = length(fragToLight);
	float shadow = 0.0;

	vec3 normal = normalize(vertexData.N);
	vec3 lightDir = normalize(fragToLight);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);;
	
	if (pcfKernel > 0)
	{
		int samples = 20;
		float viewDistance = length(viewPos - fragPos);
		float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
		for (int i = 0; i < samples; ++i)
		{
			float closestDepth = texture(shadowMap_point, fragToLight + gridSamplingDisk[i] * diskRadius).r;
			closestDepth *= farPlane;   // undo mapping [0;1]
			if (currentDepth - bias > closestDepth)
				shadow += 1.0;
		}
		shadow /= float(samples);
	}
	else
	{
		float closestDepth = texture(shadowMap_point, fragToLight).r;
		closestDepth *= farPlane;
		shadow = (currentDepth - bias > closestDepth) ? 1.0f : 0.0f;
	}
	return shadow;
}

void main()
{           
	vec4 texValue = texture(diffuseTexture, vertexData.texcoord);
    vec3 color = texValue.rgb;
	float alpha = texValue.a;
	
	if(alpha < 0.5)
		discard;

    // Ambient
    vec3 ambient = 0.2 * color;
	
    // Diffuse
    vec3 lightDir = normalize(lightPos - vertexData.worldSpacePos);
    vec3 normal = normalize(vertexData.N);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
	
    // Specular
    vec3 viewDir = normalize(viewPos - vertexData.worldSpacePos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
	
	// Shadow
	float shadow = 0.0f;        
	if (lightSourceType == 0)
	{
		shadow = shadowContribution_directionalLight(vertexData.lightSpacePos);
	}
	else if (lightSourceType == 1)
	{
		shadow = shadowContribution_pointLight(vertexData.worldSpacePos);
	}

	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    fragColor = vec4(lighting, alpha);

	//  Fog
	float dist = length(vertexData.viewSpacePos);
	if (fog_Type == 1) // Linear
	{
		fogFactor = (fog_end - dist) / (fog_end - fog_start);
	}
	else if (fog_Type == 2) // Exponential
	{
		fogFactor = 1.0 / exp(dist * fogDensity);
	}
	else if (fog_Type == 3) // Exponential sqared
	{
		fogFactor = 1.0 / exp(dist * fogDensity * dist * fogDensity);
	}
	else // Without fog
	{
		fogFactor = 1;
	}

// Distance visualization
#if 0
	if (dist < fog_start)
		fragColor = mix(fragColor, vec4(1, 0, 0, 1), 0.2);
	else if(dist >= fog_start && dist < fog_end)	 
		fragColor = mix(fragColor, vec4(0, 1, 0, 1), 0.2);
	else											 
		fragColor = mix(fragColor, vec4(0, 0, 1, 1), 0.2);
#endif

	fogFactor = clamp(fogFactor, 0.0, 1.0);
	fragColor = mix(fogColor, fragColor, fogFactor);
	if (fog_Type == 4) // Atmospheric effects
	{
		float dY = max(0.0, viewPos.y - vertexData.viewSpacePos.y);
		float b1 = dY * bi_be_factor.x;
		float b2 = dY * bi_be_factor.y;
		float ext = exp(-dist * b1);
		float insc = exp(-dist * b2);
		fragColor = fragColor * ext + fogColor * (1 - insc);
		fragColor.a = 1;
	}
}