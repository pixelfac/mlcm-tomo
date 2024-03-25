#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

layout(location = 0) out vec4 FragColor;

uniform vec2 u_resolution;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
in float vertexID;

void main()
{
	FragColor = vec4(vertexID,0.5,0.5,0.5); // vec4(r,g,b,?)
}

// TODO
// import slope and intercept for upper and lower lines
// get corner coords of given pixel
// func to get y given x,slope,intercept
// compute area under upper line
// compute area under lower line