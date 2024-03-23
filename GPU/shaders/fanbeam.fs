#version 330 core
out vec4 FragColor;

uniform float subjectResolution;

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    vec2 st = gl_FragCoord.xy/subjectResolution;
	FragColor = vec4(0.0,st.x,st.y,1.0); // vec4(r,g,b,?)
}