#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 UV;

uniform sampler2D renderedTexture;

void main(){
    FragColor = texture(renderedTexture, UV);
    // FragColor = vec4(texture(renderedTexture, UV).xy, vec2(1.0,1.0));
    // FragColor = vec4(UV.x, UV.y, 1.0, 0.0);
}