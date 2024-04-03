#version 330 core

// input 3 vertices of triangle primitive
layout (GL_TRIANGLES) in;

// output flat array of 3 vertices: src, destLeft, destRight
layout (triangle_strip, max_vertices = 3) out;

flat out vec2 positions[3];

void main() {    
    positions[0] = gl_in[0].gl_Position
    EmitVertex();

    positions[1] = gl_in[1].gl_Position;
    EmitVertex();

    positions[2] = gl_in[2].gl_Position;
    EmitVertex();
    
    EndPrimitive();
}
