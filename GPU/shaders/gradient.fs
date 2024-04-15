#version 330 core

in vec4 vecPos;

layout(location = 0) out vec3 FragColor;

void main()
{
	FragColor = vecPos.xyz;
}