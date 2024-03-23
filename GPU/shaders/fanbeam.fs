#version 330 core
out vec4 FragColor;

// make uniform for the size of subject in pixels (just the )

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    vec2 st = gl_FragCoord.xy/600;
	FragColor = vec4(st.x,st.y,0.0,1.0);
}