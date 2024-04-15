#version 330 core

layout(location = 0) out vec3 FragColor;

in vec2 UV;

uniform sampler2D rendered_texture;

void main(){
    FragColor = texture(rendered_texture, UV).xyz;
    // FragColor = vec3(texture(rendered_texture, UV).xy, 0.5);
    // FragColor = vec3(UV.x, UV.y, 0.5);
    // FragColor = vec3(1.0);
}
