#version 430 core

in vec2 o_texCoord;
in vec3 o_position;
in vec4 o_color;
in vec3 o_normal;
in vec3 o_modelPosition;

out vec4 FragColor;

uniform vec4 u_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main() {
    FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);
}
