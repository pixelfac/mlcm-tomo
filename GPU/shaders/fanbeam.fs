#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

layout(location = 0) out vec4 FragColor;

uniform vec2 u_resolution;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
in float vertexID;

in vec2 upper_line; // x is slope, y is intercept
in vec2 lower_line; // x is slope, y is intercept
flat in vec2 sourcePos;
flat in vec2 detectorLeftPos;
flat in vec2 detectorRightPos;

float upper_line_at(float x);
float lower_line_at(float x);

void main()
{
	FragColor = vec4(sourcePos.x,sourcePos.x,sourcePos.x,0.5); // vec4(r,g,b,a)
}

float upper_line_at(float x)
{
    return upper_line.x * x + upper_line.y; // y = mx + b
}

float lower_line_at(float x)
{
    return lower_line.x * x + lower_line.y; // y = mx + b
}


// TODO
// import slope and intercept for upper and lower lines
// get corner coords of given pixel
// func to get y given x,slope,intercept
// compute area under upper line
// compute area under lower line

