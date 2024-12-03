#version 430 core

in vec2 o_texCoord;
in vec3 o_position;
in vec4 o_color;
in vec3 o_normal;
in vec3 o_modelPosition;

out vec4 FragColor;

// Albedo color multiplier. Albedo means base color here.
uniform vec4 u_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main() {
    vec4 tex = vec4(1.0f);

    tex = texture(u_material.diffuse, o_texCoord).rgba;
    FragColor = mix(tex, o_color * u_color, 0.4);
}
