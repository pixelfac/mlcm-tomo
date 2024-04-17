#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 UV;

uniform sampler2D rendered_texture;

void main(){
    FragColor = texture(rendered_texture, UV);
    // FragColor = vec4(texture(rendered_texture, UV).xyz, 1.0);
    // FragColor = vec4(UV.x, UV.y, 1.0, 0.0);
}