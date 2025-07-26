#version 430 core

uniform vec4 u_color = vec4(1.0);

out vec4 outColor;

void main() {
    outColor = u_color * vec4(1.0, 1.0, 1.0, 1.0);
}
