#version 330 core

// input 3 vertices of triangle primitive
layout (triangles) in;

// output flat array of 3 vertices: src, destLeft, destRight
layout (triangle_strip, max_vertices = 3) out;

uniform int numDetectors;

flat out vec2 positions[3];

void main() {    
    for (int i = 0; i < gl_in.length(); i++)
    {
        positions[i] = gl_in[i].gl_Position.xy;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    
    EndPrimitive();
}