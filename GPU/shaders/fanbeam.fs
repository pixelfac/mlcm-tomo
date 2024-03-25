#version 330 core
layout(location = 0) out vec4 FragColor;

uniform float subjectResolution;


in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
	FragColor = vec4(0.3,0.5,0.5,0.5); // vec4(r,g,b,?)
}