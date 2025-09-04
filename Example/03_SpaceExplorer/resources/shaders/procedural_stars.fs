#version 430 core

out vec4 FragColor;

in vec2 o_texCoord;
uniform vec2 u_camera;          // for moving across space
uniform float u_time = 0.0f;    // for animating star brightness

/**
 * Pseudo randomness
 */
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float starfield(vec2 uv) {
    vec2 grid = floor(uv);
    vec2 local = fract(uv);

    float rnd = hash(grid);

    // Small probability that star exists
    float star = step(0.995, rnd); // only ~0.5% of cells contain stars

    // Vary brightness
    float brightness = mix(0.3, 1.0, hash(grid + 1.23));

    // Make stars circular (optional soft edges)
    float dist = length(local - 0.5);
    float falloff = smoothstep(0.5, 0.0, dist);

    return star * brightness * falloff;
}

void main() {
    // Transform by camera (parallax factor < 1.0 → stars move slower)
    float parallaxFactor = 0.1f;
    vec2 uv = (o_texCoord * 100.0f) + u_camera * parallaxFactor;

    // Generate star intensity
    float stars = starfield(uv);

    // Optional twinkling
    stars *= (0.8f + 0.2f * sin(u_time + uv.x * 5.0f));

    FragColor = vec4(vec3(stars), 1.0f);
}
