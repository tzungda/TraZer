#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;
layout(invocations = 6) in;

in vec2 TexCoords[3];
uniform mat4 shadowMatrices[6];

out vec2 FragTexCoords;
out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    gl_Layer = gl_InvocationID; // built-in variable that specifies to which face we render.
    for(int i = 0; i < 3; ++i) // for each triangle's vertices
    {
        FragPos = gl_in[i].gl_Position;
		FragTexCoords = TexCoords[i];
        gl_Position = shadowMatrices[gl_InvocationID] * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
} 