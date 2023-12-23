#version 430 core

//in vec3 o_textureDirection;
layout(location = 0) uniform samplerCube cubemap;

in vec3 o_position;

out vec4 FragColor;

void main() {
    FragColor =
//    vec4(0.0f, 0.4f, 1.0f, 1.0f);
    vec4(texture(cubemap, o_position).rgb, 1.0);
}
