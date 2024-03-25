#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vertexColor; // specify a color output to the fragment shader
out float vertexID;

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
    vertexID = gl_VertexID;
}

// TODO
// update gl_Position based on the current state of the CTScanner
// using code like GetCurrentSourcePosition() and GetCurrentDetectorPosition() from CTScanner.h
// determine whether vertex is source, dest_upper, dest_lower using gl_VertexID