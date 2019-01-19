#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 0.05f;
    ParticleColor = color;
    gl_Position = projection * model * view* vec4((vertex.xyz * scale) + offset, 1.0);
}
