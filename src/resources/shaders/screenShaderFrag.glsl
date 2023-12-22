#version 430 core

in vec2 o_texCoord;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
};
uniform Material u_material;

void main() {
    FragColor = texture(u_material.diffuse, o_texCoord);
}
