#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vecPos;
out vec2 UV;

void main()
{
    gl_Position = vec4(aPos, 1.0); // default value
    vecPos = (vec4(aPos, 1.0) + 1.0) / 2.0;
    UV = vecPos.xy;
}