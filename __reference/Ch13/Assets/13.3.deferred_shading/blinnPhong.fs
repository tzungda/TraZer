#version 410

layout(location = 0) out vec4 fragColor;

in VertexData
{
    vec3 N; // eye space normal
    vec3 L; // eye space light vector
    vec3 H; // eye space halfway vector
    vec2 texcoord;
	vec3 worldSpacePos;
	vec4 lightSpacePos;
} vertexData;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform int useShadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int useBias;
uniform int pcfKernel;

float shadowContribution(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vertexData.N);
    vec3 lightDir = normalize(lightPos - vertexData.worldSpacePos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	if(useBias == 0)
		bias = 0;
    // check whether current frag pos is in shadow
    
    float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	if(pcfKernel > 0)
	{
		// PCF
		for(int x = -pcfKernel; x <= pcfKernel; ++x)
		{
			for(int y = -pcfKernel; y <= pcfKernel; ++y)
			{
				float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
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
	if(useShadowMap > 0)
	{
		shadow = shadowContribution(vertexData.lightSpacePos);
	}

	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    fragColor = vec4(lighting, alpha);
}