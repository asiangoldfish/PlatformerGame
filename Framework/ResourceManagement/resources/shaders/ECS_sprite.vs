#version 430 core

// Vertex attributes
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in vec3 a_normal;

// Model - view - projection
uniform mat4 u_view = mat4(1.0f);
uniform mat4 u_projection = mat4(1.0f);
uniform mat4 u_model = mat4(1.0f);

// Output variables down the OpenGL pipeline...
out vec3 o_position;
out vec4 o_color;
out vec2 o_texCoord;

void main() {
    o_color = a_color;
    o_texCoord = a_texCoord;
    o_position = a_position;

    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
