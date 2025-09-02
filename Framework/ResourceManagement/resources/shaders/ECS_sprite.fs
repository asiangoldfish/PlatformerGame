#version 430 core

in vec3 o_position;
in vec4 o_color;
in vec2 o_texCoord;

out vec4 FragColor;

uniform vec4 u_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

struct Material {
    sampler2D diffuse;
};
uniform Material u_material;

void main() {
    vec4 tex = texture(u_material.diffuse, o_texCoord).rgba;

    FragColor = tex * u_color;
}
