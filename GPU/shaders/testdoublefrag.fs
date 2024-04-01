#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;

void main(){
    FragColor = texture(renderedTexture, vec2(UV.y, UV.x));
}