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


uniform vec3 lightPos;
uniform vec3 viewPos;


uniform sampler2D ambientTexture;
uniform bool hasAmbientTexture;

uniform sampler2D diffuseTexture;
uniform bool hasDiffuseTexture;

uniform sampler2D specularTexture;
uniform bool hasSpecularTexture;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main()
{           
	vec4 texValue = texture(diffuseTexture, vertexData.texcoord);
    vec3 color = texValue.rgb;
	float alpha = texValue.a;
	
	if(alpha < 0.5)
		discard;

    // Ambient
	
    vec3 t_ambient = (hasAmbientTexture) ? texture(ambientTexture, vertexData.texcoord).rgb : ambient;
	
    // Diffuse
	vec3 kd = (hasDiffuseTexture) ? texture(diffuseTexture, vertexData.texcoord).rgb : diffuse; 
	//kd = vec3(1);
    vec3 lightDir = normalize(lightPos - vertexData.worldSpacePos);
    vec3 normal = normalize(vertexData.N);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 t_diffuse = kd * diff;
	
    // Specular
	vec3 ks = (hasSpecularTexture) ? texture(specularTexture, vertexData.texcoord).rgb : specular; 
    vec3 viewDir = normalize(viewPos - vertexData.worldSpacePos);
    vec3 reflectDir = normalize(2.0 * normal * dot(normal, lightDir) - lightDir);
	float spec = max(dot(reflectDir, viewDir), 0.0);
    vec3 t_specular = ks * pow(spec, shininess);  

	vec3 lighting = t_ambient + t_diffuse + t_specular;    
    fragColor = vec4(lighting, alpha);
}