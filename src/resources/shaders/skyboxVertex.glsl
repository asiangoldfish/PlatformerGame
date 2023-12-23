#version 430 core

// Vertex attributes
layout(location = 0) in vec3 a_position;

out vec3 o_position;

// Model - view - projection
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main() {
    o_position = a_position;
    gl_Position =  u_projection * u_view * u_model * vec4(a_position, 1.0);
}
