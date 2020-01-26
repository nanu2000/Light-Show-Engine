#version 330 core

uniform mat4 shadowMatrices[6];

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;
out vec4 FragPosition_o; // FragPos from GS (output per emitvertex)

void main() {
    for (int face = 0; face < 6; ++face) {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for (int i = 0; i < 3; ++i) // for each triangle's vertices
        {

            FragPosition_o = gl_in[i].gl_Position;
            gl_Position    = shadowMatrices[face] * FragPosition_o;
            EmitVertex();
        }
        EndPrimitive();
    }
}
