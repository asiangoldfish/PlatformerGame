#version 430 core

// Vertex attributes
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in vec3 a_normal;

// Model - view - projection
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

// Output variables down the OpenGL pipeline..
void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
