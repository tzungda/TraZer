#version 410

layout(location = 0) out vec4 fragColor;

uniform vec3 lineColor;

void main()
{           
    fragColor = vec4(lineColor, 1.0 );
}