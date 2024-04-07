#version 330 core

layout(location = 0) out vec4 FragColor;

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

in vec2 UV;

uniform int u_resolution;

out vec4 color;

uniform sampler2D renderedTexture;

void main(){
    FragColor = texture(renderedTexture, vec2(UV.x, UV.y));
    // FragColor = vec4(UV.x, UV.y, 1.0, 0.0);
}