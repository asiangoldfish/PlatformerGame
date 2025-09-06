#version 430 core

in vec2 o_texCoord;
out vec4 FragColor;

uniform vec4 u_color;
uniform float u_radius = 0.5f; // normalized [0, 0.5] (since UVs are 0–1)
uniform float u_thickness = 0.01f; // thickness of the ring, e.g. 0.02

void main() {
    vec2 center = vec2(0.5, 0.5);
    float dist = length(o_texCoord - center);

    // Draw only if distance is within [radius-thickness/2, radius+thickness/2]
    float edge = smoothstep(u_radius - u_thickness, u_radius - u_thickness * 0.5, dist) *
                 (1.0 - smoothstep(u_radius + u_thickness * 0.5, u_radius + u_thickness, dist));

    FragColor = vec4(u_color.rgb, u_color.a * edge);
}
