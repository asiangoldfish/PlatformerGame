#version 430 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;

out vec2 o_texCoord;

void main()
{
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
    o_texCoord = a_texCoord;
}
